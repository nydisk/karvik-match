#pragma once
#include "core/transition.hpp"
#include "core/settings.hpp"
#include "ext/ini.h"
#include "core/registry.hpp"
class Config {
	inline static int getIntFromINIWithHandling(const mINI::INIStructure& ini, const std::string& cat, const std::string& id, const int defaultValue) {
		if (!ini.has(cat)) {
			MessageBoxA(nullptr, std::string("configuration error: category '" + cat + "' does not exist.").c_str(), "karvik-match cfg loader", MB_OK);
			return defaultValue;
		}
		if (!ini.get(cat).has(id)) {
			MessageBoxA(nullptr, std::string("configuration error: param '" + id + "' does not exist within category '" + cat + "'.").c_str(), "karvik-match cfg loader", MB_OK);
			return defaultValue;
		}
		try {
			int value = std::stoi(ini.get(cat).get(id));
			return value;
		}
		catch (...) {
			MessageBoxA(nullptr, std::string("configuration error: param '" + id + "' in '" + cat + "' has an invalid datatype (must be int)").c_str(), "karvik-match cfg loader", MB_OK);
			return defaultValue;
		}
	}
public:
	inline static void changeResolution(const sf::Vector2u& newSize) {
		if (Settings::MAIN_WINDOW == nullptr) return;
		if (Settings::MAIN_RENDERTARGET == nullptr) return;
		if (Settings::MAIN_RENDERSHAPE == nullptr) return;

		Settings::VIRTUAL_WIDTH = static_cast<float>(newSize.x);
		Settings::VIRTUAL_HEIGHT = static_cast<float>(newSize.y);

		Settings::SCALED_FONT_SIZE = static_cast<unsigned int>((Settings::VIRTUAL_HEIGHT / 512.0F) * Settings::STANDARD_FONT_SIZE);
		Settings::MAIN_WINDOW = std::make_unique<sf::RenderWindow>(sf::VideoMode(newSize), ":3");

		Settings::MAIN_RENDERTARGET = std::make_unique<sf::RenderTexture>(Settings::MAIN_WINDOW->getSize());
		Settings::MAIN_RENDERSHAPE = std::make_unique<sf::RectangleShape>(sf::Vector2f{ static_cast<float>(Settings::MAIN_WINDOW->getSize().x), static_cast<float>(Settings::MAIN_WINDOW->getSize().y) });

		Transition::reloadShape();
	}
	inline static void changeVolume(const float master = -1.0F, const float music = -1.0F, const float sfx = -1.0F) {
		if (master >= 0.0F) Settings::MASTER_VOLUME = std::clamp(master, 0.0F, 100.0F);
		if (music >= 0.0F) Settings::MUSIC_VOLUME = std::clamp(music, 0.0F, 100.0F);
		if (sfx >= 0.0F) Settings::SFX_VOLUME = std::clamp(sfx, 0.0F, 100.0F);

		Registry::updateVolumes();
	}

	inline static bool saveSettings() {
		mINI::INIFile file("cfg.ini");
		mINI::INIStructure ini;

		ini["graphics"]["width"] = std::to_string(static_cast<int>(Settings::VIRTUAL_WIDTH));
		ini["graphics"]["height"] = std::to_string(static_cast<int>(Settings::VIRTUAL_HEIGHT));

		ini["audio"]["master"] = std::to_string(static_cast<int>(Settings::MASTER_VOLUME));
		ini["audio"]["music"] = std::to_string(static_cast<int>(Settings::MUSIC_VOLUME));
		ini["audio"]["sfx"] = std::to_string(static_cast<int>(Settings::SFX_VOLUME));

		return file.generate(ini, true);
	}

	
	inline static bool loadSettings() {
		mINI::INIFile file("cfg.ini");
		mINI::INIStructure ini;
		if (!file.read(ini)) {
			return false;
		}

		Settings::VIRTUAL_WIDTH = static_cast<float>(getIntFromINIWithHandling(ini, "graphics", "width", 512));
		Settings::VIRTUAL_HEIGHT = static_cast<float>(getIntFromINIWithHandling(ini, "graphics", "height", 512));

		Settings::MASTER_VOLUME = static_cast<float>(std::clamp(getIntFromINIWithHandling(ini, "audio", "master", 100), 0, 100));
		Settings::MUSIC_VOLUME = static_cast<float>(std::clamp(getIntFromINIWithHandling(ini, "audio", "music", 100), 0, 100));
		Settings::SFX_VOLUME = static_cast<float>(std::clamp(getIntFromINIWithHandling(ini, "audio", "sfx", 100), 0, 100));

		Settings::SCALED_FONT_SIZE = static_cast<unsigned int>((Settings::VIRTUAL_HEIGHT / 512.0F) * Settings::STANDARD_FONT_SIZE);
		return true;
	}
};
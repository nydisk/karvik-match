#pragma once
#include "transition.hpp"
#include "settings.hpp"
#include "ext/ini.h"
class Config {
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

	inline static bool saveSettings() {
		mINI::INIFile file("cfg.ini");
		mINI::INIStructure ini;

		ini["graphics"]["width"] = std::to_string(static_cast<int>(Settings::VIRTUAL_WIDTH));
		ini["graphics"]["height"] = std::to_string(static_cast<int>(Settings::VIRTUAL_HEIGHT));

		return file.generate(ini, true);
	}

	inline static bool loadSettings() {
		mINI::INIFile file("cfg.ini");
		mINI::INIStructure ini;
		if (!file.read(ini)) {
			return false;
		}
		if (ini["graphics"].has("width")) {
			int width = std::stoi(ini["graphics"]["width"]);
			if (width > 0) {
				Settings::VIRTUAL_WIDTH = static_cast<float>(width);
			}
		}
		if (ini["graphics"].has("height")) {
			int height = std::stoi(ini["graphics"]["height"]);
			if (height > 0) {
				Settings::VIRTUAL_HEIGHT = static_cast<float>(height);
			}
		}
		Settings::SCALED_FONT_SIZE = static_cast<unsigned int>((Settings::VIRTUAL_HEIGHT / 512.0F) * Settings::STANDARD_FONT_SIZE);
		return true;
	}
};
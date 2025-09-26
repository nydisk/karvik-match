#pragma once
#include "scene.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
class Settings {
public:
	inline static float VIRTUAL_WIDTH = 1280;
	inline static float VIRTUAL_HEIGHT = 720;
	inline static constexpr unsigned int STANDARD_FONT_SIZE = 18u;

	// 512 is the height of the reference resolution for the font size
	inline static unsigned int SCALED_FONT_SIZE = static_cast<unsigned int>((Settings::VIRTUAL_HEIGHT / 512.0F) * Settings::STANDARD_FONT_SIZE);
	inline static constexpr const char* VERSION_STRING = "v0.3-alpha";
	inline static constexpr SceneId INITIAL_SCENE = SceneId::Settings;

	inline static std::unique_ptr<sf::RenderWindow> MAIN_WINDOW = nullptr;
	inline static std::unique_ptr<sf::RenderTexture> MAIN_RENDERTARGET = nullptr;
	inline static std::unique_ptr<sf::RectangleShape> MAIN_RENDERSHAPE = nullptr;

	inline static void changeResolution(const sf::Vector2u& newSize){
		if (MAIN_WINDOW == nullptr) return;
		if (MAIN_RENDERTARGET == nullptr) return;
		if (MAIN_RENDERSHAPE == nullptr) return;

		VIRTUAL_WIDTH = static_cast<float>(newSize.x);
		VIRTUAL_HEIGHT = static_cast<float>(newSize.y);
		
		SCALED_FONT_SIZE = static_cast<unsigned int>((Settings::VIRTUAL_HEIGHT / 512.0F) * Settings::STANDARD_FONT_SIZE);

		MAIN_WINDOW = std::make_unique<sf::RenderWindow>(sf::VideoMode(newSize), ":3");
		MAIN_RENDERTARGET = std::make_unique<sf::RenderTexture>(MAIN_WINDOW->getSize());
		MAIN_RENDERSHAPE = std::make_unique<sf::RectangleShape>(sf::Vector2f{ static_cast<float>(MAIN_WINDOW->getSize().x), static_cast<float>(MAIN_WINDOW->getSize().y) });
	}
};
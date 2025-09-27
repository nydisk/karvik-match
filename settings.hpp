#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "scene.hpp"
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
};
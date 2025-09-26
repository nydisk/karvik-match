#pragma once
#include "scene.hpp"
class Settings {
public:
	inline static constexpr float VIRTUAL_WIDTH = 1280;
	inline static constexpr float VIRTUAL_HEIGHT = 720;
	inline static constexpr unsigned int STANDARD_FONT_SIZE = 18u;
	// 512 is the height of the reference resolution for the font size
	inline static constexpr unsigned int SCALED_FONT_SIZE = static_cast<unsigned int>((Settings::VIRTUAL_HEIGHT / 512.0F) * Settings::STANDARD_FONT_SIZE);
	inline static constexpr const char* VERSION_STRING = "v0.3-alpha";
	inline static constexpr SceneId INITIAL_SCENE = SceneId::Settings;
};
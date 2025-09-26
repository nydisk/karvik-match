#pragma once

class Settings {
public:
	inline static constexpr float VIRTUAL_WIDTH = 1920;
	inline static constexpr float VIRTUAL_HEIGHT = 1080;
	inline static constexpr unsigned int STANDARD_FONT_SIZE = 18u;
	inline static constexpr unsigned int SCALED_FONT_SIZE = static_cast<unsigned int>((Settings::VIRTUAL_HEIGHT / 512.0F) * Settings::STANDARD_FONT_SIZE);
	inline static constexpr float WINDOW_MULTIPLIER = 1.0F;
	inline static constexpr const char* VERSION_STRING = "v0.3-alpha";
};
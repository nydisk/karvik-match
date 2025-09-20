#pragma once
#include <SFML/Audio.hpp>
#include "registry.hpp"
class SFX {
public:
	inline static void play(const std::string& bufferId) {
		auto& snd = Registry::getSound(bufferId);
		snd.play();
	}
};
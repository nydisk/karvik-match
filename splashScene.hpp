#pragma once
#include "scene.hpp"

class SplashScene : public Scene {
public:
	SplashScene() : Scene(SceneId::Splash) {}
	void update(sf::RenderWindow& window, const TimeSnap& time) override {
	
	}
	void draw(sf::RenderWindow& window, const TimeSnap& time) override {
	
	}
	void onSFMLEvent(const std::optional<sf::Event>& ev) override {
	
	}
	void onUnload() override {
	
	}
	void onLoad() override {
	
	}
};
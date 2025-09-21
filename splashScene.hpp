#pragma once
#include "scene.hpp"

class SplashScene : public Scene {
public:
	SplashScene() : Scene(SceneId::Splash) {}
	void update(sf::RenderTarget& target, sf::RenderWindow& window, const TimeSnap& time) override {
	
	}
	void draw(sf::RenderTarget& target, sf::RenderWindow& window, const TimeSnap& time) override {
	
	}
	void onSFMLEvent(const std::optional<sf::Event>& ev) override {
	
	}
	void onUnload() override {
		Scene::onUnload();
	}
	void onLoad() override {
		Scene::onLoad();
	}
};
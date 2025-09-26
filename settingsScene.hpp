#pragma once
#include "sfx.hpp"
#include "scene.hpp"
#include "scenery.hpp"
#include "registry.hpp"
#include <SFML/Graphics.hpp>

class SettingsScene : public Scene {
public:
	SettingsScene() : Scene(SceneId::Settings) {}
	void update(sf::RenderTarget& target, sf::RenderWindow& window, const TimeSnap& time) override {}
	void draw(sf::RenderTarget& target, sf::RenderWindow& window, const TimeSnap& time) override {}
	void onLoad() override { Scene::onLoad(); }
	void onUnload() override { Scene::onUnload(); }
	void onSFMLEvent(const std::optional<sf::Event>& ev) override {}
};
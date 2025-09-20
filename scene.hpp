#pragma once
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include "timesnap.hpp"
enum class SceneId : int {
	Splash,
	Menu,
	Game,
	Results
};
class Scene {
protected:
	SceneId m_id;
public:
	Scene(const SceneId id);
	virtual void update(sf::RenderWindow& window, const TimeSnap& time) = 0;
	virtual void draw(sf::RenderWindow& window, const TimeSnap& time) = 0;
	virtual void onLoad();
	virtual void onUnload();
};
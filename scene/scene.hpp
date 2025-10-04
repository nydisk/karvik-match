#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>
#include "util/timesnap.hpp"
enum class SceneId : int {
	Splash,
	Menu,
	Game,
	Settings
};
class Scene {
private:
	inline static const std::unordered_map<std::string, SceneId> m_sceneNameDict{
		{"splash", SceneId::Splash},
		{"menu", SceneId::Menu},
		{"game", SceneId::Game},
		{"settings", SceneId::Settings},
	};
protected:
	SceneId m_id;
public:
	inline static SceneId getSceneIdFromName(const std::string id) {
		return m_sceneNameDict.at(id);
	}
	Scene(const SceneId id);
	virtual void update(sf::RenderTarget& renderTarget, sf::RenderWindow& window, const TimeSnap& time) = 0;
	virtual void draw(sf::RenderTarget& renderTarget, sf::RenderWindow& window, const TimeSnap& time) = 0;
	virtual void onSFMLEvent(const std::optional<sf::Event>& ev) = 0;
	virtual void onLoad();
	virtual void onUnload();
};
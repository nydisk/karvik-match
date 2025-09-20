#pragma once
#include "scene.hpp"
class GameScene : public Scene {
public:
	GameScene() : Scene(SceneId::Game) {}
};
#include "scene.hpp"
#include "sound/bgm.hpp"

Scene::Scene(const SceneId id) : m_id(id) {}

void Scene::onLoad() {
	BGM::play();
	std::cout << "scene-" << static_cast<int>(m_id) << ": Loaded scene" << std::endl;
}

void Scene::onUnload() {
	BGM::resetState();
	std::cout << "scene-" << static_cast<int>(m_id) << ": Unloaded scene" << std::endl;
}
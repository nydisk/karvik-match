#include "scene.hpp"
#include "bgm.hpp"
Scene::Scene(const SceneId id) : m_id(id) {}

void Scene::onLoad() {
	BGM::play();
	std::cout << "Loaded scene " << static_cast<int>(m_id) << std::endl;
}

void Scene::onUnload() {
	BGM::resetState();
	std::cout << "Unloaded scene " << static_cast<int>(m_id) << std::endl;
}
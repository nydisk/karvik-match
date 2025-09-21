#include "scene.hpp"
Scene::Scene(const SceneId id) : m_id(id) {}

void Scene::onLoad() {
	std::cout << "Loaded scene " << static_cast<int>(m_id) << std::endl;
}

void Scene::onUnload() {
	std::cout << "Unloaded scene " << static_cast<int>(m_id) << std::endl;
}
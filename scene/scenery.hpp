#pragma once
#include <unordered_map>
#include <functional>
#include "scene.hpp"
#include "core/transition.hpp"
class Scenery {
	inline static std::unordered_map<SceneId, std::function<std::unique_ptr<Scene>()>> m_scenes{};
	inline static std::unique_ptr<Scene> m_activeScene{};
public:
	inline static Scene* active() {
		return m_activeScene.get();
	}
	inline static void registerFactory(const SceneId id, const std::function<std::unique_ptr<Scene>()>& factory) {
		m_scenes[id] = factory;
		std::cout << "scenery: Registered scene factory " << static_cast<int>(id) << std::endl;
	}
	inline static void load(const SceneId id, const bool instant = false) {
		if (!m_activeScene) { // first scene edge case
			auto it = m_scenes.find(id);
			if (it == m_scenes.end()) throw std::runtime_error("scenery: no scene registered");
			m_activeScene = it->second();
			m_activeScene->onLoad();
			Transition::fadeIn();
			return;
		}

		if (instant) {
			if (m_activeScene) {
				m_activeScene->onUnload();
			}
			auto it = m_scenes.find(id);
			if (it == m_scenes.end()) {
				throw std::runtime_error("scenery: no scene registered");
			}
			m_activeScene = it->second();
			m_activeScene->onLoad();
			return;
		}
		else {
			Transition::fadeOut([id]() {
				if (m_activeScene) {
					m_activeScene->onUnload();
				}
				auto it = m_scenes.find(id);
				if (it != m_scenes.end()) {
					m_activeScene = it->second();
				}
				else {
					throw std::runtime_error("scenery: no scene registered");
				}
				m_activeScene = m_scenes[id]();
				m_activeScene->onLoad();
				Transition::fadeIn();
				});
		}
	}
};
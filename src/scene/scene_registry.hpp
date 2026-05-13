#pragma once
#include <functional>
#include <memory>
#include <unordered_map>

#include "scene.hpp"
#include "scene_id.hpp"

typedef std::function<std::unique_ptr<Scene>(GameContext&)> scene_factory_t;

class SceneRegistry {
    std::unordered_map<SceneId, scene_factory_t> scenes_;
    std::unique_ptr<Scene> currentScene_ = nullptr;
    SceneId queuedScene_ = SceneId::None;
public:
    void registerFactory(SceneId id, scene_factory_t factory);
    void loadScene(SceneId id);
    void loadIfQueued(GameContext& ctx);
    Scene* getCurrentScene() const;
};

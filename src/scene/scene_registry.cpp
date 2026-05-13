#include "scene_registry.hpp"
#include "spdlog/spdlog.h"

void SceneRegistry::registerFactory(SceneId id, scene_factory_t factory) {
    scenes_[id] = std::move(factory);
    spdlog::debug("registered factory for {}", static_cast<int>(id));
}

void SceneRegistry::loadScene(const SceneId id) {
    if (!scenes_.contains(id)) {
        spdlog::error("no registered factory for scene {}", static_cast<int>(id));
        return;
    }

    queuedScene_ = id;
}

void SceneRegistry::loadIfQueued(GameContext& ctx) {
    if (queuedScene_ == SceneId::None) return;

    if (currentScene_) currentScene_->unload();
    currentScene_ = scenes_.at(queuedScene_)(ctx);
    currentScene_->load();

    spdlog::debug("loaded scene {}", static_cast<int>(queuedScene_));

    queuedScene_ = SceneId::None;
}

Scene* SceneRegistry::getCurrentScene() const {
    return currentScene_.get();
}

#pragma once
#include "scene_id.hpp"

struct GameContext;

class Scene {
protected:
    GameContext& ctx_;
public:
    explicit Scene(GameContext& ctx);
    virtual ~Scene() = default;

    virtual SceneId id() = 0;

    virtual void load() = 0;
    virtual void unload() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
};

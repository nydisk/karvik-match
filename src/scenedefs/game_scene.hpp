#pragma once
#include "../scene/scene.hpp"

class GameScene : public Scene {
public:
    explicit GameScene(GameContext& ctx);

    SceneId id() override;

    void load() override;
    void unload() override;
    void update() override;
    void render() override;
};

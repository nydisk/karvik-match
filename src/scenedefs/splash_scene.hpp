#pragma once
#include "../scene/scene.hpp"
#include "raylib.h"

struct GameContext;
class TextureAsset;
class SplashScene : public Scene {
    static constexpr float SplashFade = 2.0f;
    static constexpr float SplashStay = 2.5f;
    static constexpr float SplashOffset = 0.5f;

    float timer_ = 0.0f;
    float alpha_ = 0.0f;

    TextureAsset* splashTex_ = nullptr;
    Rectangle sourceRect_{};
public:
    explicit SplashScene(GameContext& ctx);

    SceneId id() override;

    void load() override;
    void unload() override;
    void update() override;
    void render() override;
};

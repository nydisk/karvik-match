#include "splash_scene.hpp"
#include "../scene/scene_registry.hpp"
#include "../asset/asset_registry.hpp"
#include "../game/game_ctx.hpp"
#include "raymath.h"

SplashScene::SplashScene(GameContext& ctx): Scene(ctx) {}

SceneId SplashScene::id() { return SceneId::Splash; }

void SplashScene::load() {
    splashTex_ = ctx_.assets.load<TextureAsset>("splash", "splash.png");
    sourceRect_ = Rectangle(0, 0,
        static_cast<float>(splashTex_->tex().width),
        static_cast<float>(splashTex_->tex().height)
    );
}

void SplashScene::unload() {
    ctx_.assets.unload("splash");
}

void SplashScene::update() {
    if (IsKeyPressed(KEY_ESCAPE)) timer_ = 9999;

    timer_ += GetFrameTime();

    if (timer_ <= SplashFade) {
        alpha_ = timer_ / SplashFade;
    }
    else if (timer_ <= SplashFade + SplashStay) {
        alpha_ = 1.0f;
    }
    else if (timer_ <= SplashFade * 2 + SplashStay) {
        alpha_ = 1.0f - (timer_ - SplashFade - SplashStay) / SplashFade;
    }
    else {
        alpha_ = 0.0f;
    }

    if (timer_ >= SplashOffset + SplashStay + SplashFade * 2) {
        ctx_.scenes.loadScene(SceneId::Game);
    }
}

void SplashScene::render() {
    DrawTexturePro(
        splashTex_->tex(),
        sourceRect_,
        Rectangle(0, 0, static_cast<float>(ctx_.rw), static_cast<float>(ctx_.rh)),
        Vector2Zeros,
        0.0f,
        Fade(WHITE, alpha_)
    );
}

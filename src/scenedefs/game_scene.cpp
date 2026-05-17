#include "game_scene.hpp"

#include <deque>
#include <random>
#include <string>

#include "../asset/asset_registry.hpp"
#include "../asset/data_registry.hpp"
#include "../game/game_ctx.hpp"
#include "spdlog/spdlog.h"

GameScene::GameScene(GameContext& ctx) : Scene(ctx) {}

SceneId GameScene::id() { return SceneId::Game; }

void GameScene::load() {
    std::deque<std::string> dataQueue{}; // contains all the cards
    std::deque<std::string> spawnedQueue{}; // contains the cards in game (doubled)

    if (ctx_.data.cardCount() <= 0) {
        spdlog::critical("no cards");
        return;
    }

    if (ctx_.data.cardCount() < GridCX * GridCY / 2) {
        spdlog::warn("not enough cards to properly fill the grid");
    }

    std::mt19937 gen {std::random_device{}()};

    const auto& defs = ctx_.data.cardVec();
    dataQueue.assign(defs.begin(), defs.end());
    std::ranges::shuffle(dataQueue, gen);

    for (size_t i = 0; i < GridCX * GridCY / 2; i++) {
        const auto& id = dataQueue.front();
        spawnedQueue.push_back(id); spawnedQueue.push_back(id);
        dataQueue.pop_front();
    }

    std::ranges::shuffle(spawnedQueue, gen);

    Texture2D& backTex = ctx_.assets.load<TextureAsset>("cardback", "card_back.png")->tex();

    Vector2 gs = getCardAreaStart(ctx_.rw, ctx_.rh);
    for (int y = 0; y < GridCX; y++) {
        for (int x = 0; x < GridCY; x++) {
            const std::string& id = spawnedQueue.front();

            auto card = std::make_unique<Card>(gs, x, y, id);
            card->initializeContent(ctx_.assets, backTex);

            cards_[y * GridCX + x] = std::move(card);

            spawnedQueue.pop_front();
        }
    }
}

void GameScene::unload() {
    for (auto& card : cards_) card.reset();
}

void GameScene::update() {
    for (const auto& card : cards_) {
        if (IsWindowResized()) {
            card->updateGridStart(getCardAreaStart(ctx_.rw, ctx_.rh));
        }
        card->update(ctx_.rh);

        if (card->hovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            resolver_.cardClicked(card.get());
        }
    }
}

void GameScene::render() {
    for (const auto& card : cards_) card->render();
}

Vector2 GameScene::getCardAreaStart(const int rw, const int rh) {
    const auto [sx, sy] = getCardAreaSize(rh);
    return {
        0.5f * (static_cast<float>(rw) - sx),
        0.5f * (static_cast<float>(rh) - sy)
    };
}

Vector2 GameScene::getCardAreaSize(const int rh) {
    const float sx = GridCX * (Card::getCardWidth(rh) + Card::Padding) - Card::Padding;
    const float sy = GridCY * (Card::getCardHeight(rh) + Card::Padding) - Card::Padding;
    return Vector2(sx, sy);
}
#include "card.hpp"

#include "raymath.h"
#include "../asset/asset_registry.hpp"
#include "../scenedefs/game_scene.hpp"

Card::Card(const Vector2 gridStart, const int gridX, const int gridY, std::string definitionId): gs_(gridStart), gx_(gridX), gy_(gridY), definitionId_(std::move(definitionId)) {}

void Card::initializeContent(AssetRegistry& assets, Texture2D& cardbackTex) {
    texture_ = &assets.get<TextureAsset>("card_" + definitionId_)->tex();
    texSource_ = Rectangle(0, 0, static_cast<float>(texture_->width), static_cast<float>(texture_->height));
    backTexture_ = &cardbackTex;
}

void Card::render() const {
    if (texture_ == nullptr || backTexture_ == nullptr) {
        spdlog::critical("no card or cardback texture associated with {}, {}", gx_, gy_);
        return;
    }
    DrawTexturePro(state_ == CardState::Back ? *backTexture_ : *texture_, texSource_, bounds_, Vector2Zeros, 0.0f, WHITE);
}

void Card::update(const int rh) {
    auto [x, y] = getRenderPosition(rh);
    bounds_ = Rectangle{x, y, getCardWidth(rh), getCardHeight(rh)};
    hovered_ = CheckCollisionPointRec(GetMousePosition(), bounds_);
}

void Card::updateGridStart(const Vector2 gs) {
    gs_ = gs;
}

float Card::getCardHeight(const int rh) {
    return (static_cast<float>(rh) * GridHeightFraction - (GameScene::GridCY  - 1) * Padding) / GameScene::GridCY;
}

float Card::getCardWidth(const int rh) {
    return getCardHeight(rh) * (static_cast<float>(RawCardWidth) / static_cast<float>(RawCardHeight));
}

std::string Card::id() const {
    return definitionId_;
}

bool Card::hovered() const {
    return hovered_;
}

void Card::setState(const CardState state) {
    this->state_ = state;
}

Vector2 Card::getRenderPosition(const int rh) const {
    const float xo = static_cast<float>(gx_) * (getCardWidth(rh) + Padding);
    const float yo = static_cast<float>(gy_) * (getCardHeight(rh) + Padding);
    return gs_ + Vector2(xo, yo);
}

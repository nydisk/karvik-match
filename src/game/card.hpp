#pragma once
#include <string>
#include "raylib.h"

class AssetRegistry;

enum class CardState {
    Back,
    Front
};

class Card {
public:
    static constexpr int Padding = 10;
    static constexpr int RawCardWidth = 64;
    static constexpr int RawCardHeight = 96;
    static constexpr float GridHeightFraction = 0.85f;
private:
    Vector2 gs_;
    int gx_, gy_;

    std::string definitionId_;
    Texture2D* backTexture_ = nullptr;
    Texture2D* texture_ = nullptr;
    CardState state_ = CardState::Back;
    Rectangle texSource_{};
    Rectangle bounds_{};
    bool hovered_ = false;
public:
    explicit Card(Vector2 gridStart, int gridX, int gridY, std::string definitionId);

    void initializeContent(AssetRegistry& assets, Texture2D& cardbackTex);

    void render() const;

    void update(int rh);

    void updateGridStart(Vector2 gs);

    static float getCardHeight(int rh);
    static float getCardWidth(int rh);

    std::string id() const;
    bool hovered() const;
    void setState(CardState state);
private:
    [[nodiscard]] Vector2 getRenderPosition(int rh) const;
};

#pragma once
#include <string>
#include "raylib.h"

class AssetRegistry;
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
    Texture2D* texture_ = nullptr;
    Rectangle texSource_{};
public:
    explicit Card(Vector2 gridStart, int gridX, int gridY, std::string definitionId);

    void initializeContent(AssetRegistry& assets);

    void render(int rh) const;

    void updateGridStart(Vector2 gs);

    static float getCardHeight(int rh);
    static float getCardWidth(int rh);
private:
    [[nodiscard]] Vector2 getRenderPosition(int rh) const;
};

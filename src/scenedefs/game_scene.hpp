#pragma once
#include <array>
#include <memory>

#include "../game/card.hpp"
#include "../game/resolver.hpp"
#include "../scene/scene.hpp"

struct GameContext;
class GameScene : public Scene {
public:
    static constexpr int GridCX = 4;
    static constexpr int GridCY = 4;
private:
    std::array<std::unique_ptr<Card>, GridCX * GridCY> cards_{};
    Resolver resolver_{};
public:
    explicit GameScene(GameContext& ctx);

    SceneId id() override;

    void load() override;
    void unload() override;
    void update() override;
    void render() override;

private:
    static Vector2 getCardAreaSize(int rh);
    static Vector2 getCardAreaStart(int rw, int rh);
};

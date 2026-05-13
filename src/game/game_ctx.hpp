#pragma once

class SceneRegistry;
class AssetRegistry;

struct GameContext {
    AssetRegistry& assets;
    SceneRegistry& scenes;
};

#pragma once

class SceneRegistry;
class AssetRegistry;
class DataRegistry;

struct GameContext {
    AssetRegistry& assets;
    SceneRegistry& scenes;
    DataRegistry& data;

    int rw, rh;
};

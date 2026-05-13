#include "raylib.h"
#include "asset/asset_registry.hpp"
#include "game/game_ctx.hpp"
#include "scene/scene_registry.hpp"
#include "scenedefs/splash_scene.hpp"
#include "scenedefs/game_scene.hpp"

class GameScene;

void initializeAssets(AssetRegistry& registry);
void initializeScenes(SceneRegistry& registry);

int main(){
	spdlog::set_level(spdlog::level::debug);

	SetTraceLogLevel(LOG_WARNING);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "karvikmatch");

	AssetRegistry assets{};
	SceneRegistry scenes{};
	GameContext ctx{assets,scenes};

	initializeAssets(assets);
	initializeScenes(scenes);

	scenes.loadScene(SceneId::Splash);
	scenes.loadIfQueued(ctx);

	while (!WindowShouldClose()) {
		Scene* scene = scenes.getCurrentScene();
		if (scene) scene->update();

		BeginDrawing();
		ClearBackground(BLACK);
		if (scene) scene->render();
		EndDrawing();

		scenes.loadIfQueued(ctx);
	}
}

void initializeAssets(AssetRegistry& registry) {
	registry.registerSimpleFactory<TextureAsset>();
	spdlog::info("finished loading assets");
}

void initializeScenes(SceneRegistry& registry) {
	registry.registerSimpleFactory<SplashScene>(SceneId::Splash);
	registry.registerSimpleFactory<GameScene>(SceneId::Game);
	spdlog::info("finished initializing scenes");
}
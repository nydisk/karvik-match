#include "raylib.h"
#include "asset/asset_registry.hpp"
#include "asset/data_registry.hpp"
#include "asset/wren_importer.hpp"
#include "game/game_ctx.hpp"
#include "scene/scene_registry.hpp"
#include "scenedefs/game_scene.hpp"
#include "scenedefs/splash_scene.hpp"

void initializeAssets(AssetRegistry& registry);
void initializeScenes(SceneRegistry& registry);

int main(){
	//spdlog::set_level(spdlog::level::debug);

	SetTraceLogLevel(LOG_WARNING);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "karvikmatch");

	const WrenImporter wren{};

	AssetRegistry assets{};
	SceneRegistry scenes{};

	DataRegistry data(assets);
	wrenSetUserData(wren.vm(), &data);

	GameContext ctx{assets,scenes,data};

	initializeAssets(assets);
	wren.interpretWrenFile("cards", "cards.wren");
	spdlog::info("loaded {} card(s)", data.cardCount());
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

	wren.free();
}

void initializeAssets(AssetRegistry& registry) {
	registry.registerSimpleFactory<TextureAsset>();
	spdlog::info("finished loading asset factories");
}

void initializeScenes(SceneRegistry& registry) {
	registry.registerSimpleFactory<SplashScene>(SceneId::Splash);
	registry.registerSimpleFactory<GameScene>(SceneId::Game);
	spdlog::info("finished initializing scenes");
}
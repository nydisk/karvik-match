#include "raylib.h"
#include "raymath.h"
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
	spdlog::set_level(spdlog::level::debug);

	SetTraceLogLevel(LOG_WARNING);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(960, 540, "karvikmatch");
	SetExitKey(0);

	RenderTexture2D rtex = LoadRenderTexture(
		GetRenderWidth(),
		GetRenderHeight()
	);
	SetTextureFilter(rtex.texture, TEXTURE_FILTER_POINT);
	
	const WrenImporter wren{};

	AssetRegistry assets{};
	SceneRegistry scenes{};

	DataRegistry data(assets);
	wrenSetUserData(wren.vm(), &data);

	GameContext ctx{
		assets,
		scenes,
		data,
		GetRenderWidth(),
		GetRenderHeight()
	};

	initializeAssets(assets);
	wren.interpretWrenFile("cards", "cards.wren");
	spdlog::info("loaded {} card(s)", data.cardCount());
	initializeScenes(scenes);

	scenes.loadScene(SceneId::Splash);
	scenes.loadIfQueued(ctx);

	while (!WindowShouldClose()) {
		if (IsWindowResized()) {
			UnloadRenderTexture(rtex);
			rtex = LoadRenderTexture(
				GetRenderWidth(),
				GetRenderHeight()
			);
			ctx.rw = rtex.texture.width;
			ctx.rh = rtex.texture.height;
			SetTextureFilter(rtex.texture, TEXTURE_FILTER_POINT);
		}

		Scene* scene = scenes.getCurrentScene();
		if (scene) scene->update();

		BeginTextureMode(rtex);
		ClearBackground(BLACK);
		if (scene) scene->render();
		EndTextureMode();

		BeginDrawing();
		ClearBackground(BLACK);
		DrawTexturePro(
			rtex.texture,
			Rectangle(0, 0,
				static_cast<float>(rtex.texture.width),
				-static_cast<float>(rtex.texture.height)
			),
			Rectangle(0, 0,
				static_cast<float>(GetRenderWidth()),
				static_cast<float>(GetRenderHeight())
			), Vector2Zeros, 0.0f, WHITE
		);
		EndDrawing();

		scenes.loadIfQueued(ctx);
	}

	wren.free();
	UnloadRenderTexture(rtex);
	CloseWindow();
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
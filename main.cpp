#define _USE_MATH_DEFINES
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <math.h>
#include <SFML/Graphics.hpp>
#include "settingsScene.hpp"
#include "splashScene.hpp"
#include "transition.hpp"
#include "gameScene.hpp"
#include "menuScene.hpp"
#include "registry.hpp"
#include "settings.hpp"
#include "timesnap.hpp"
#include "manifest.hpp"
#include "scenery.hpp"

std::vector<std::string> loadedCards{};

static void assetLoadError(const std::string& target_id, const std::string& path_searched){
	std::ostringstream oss{};
	oss << "failed to load registry item '";
	oss << target_id;
	oss << "':\ncouldn't locate file '";
	oss << path_searched;
	oss << "'";

	MessageBoxA(nullptr, oss.str().c_str(), "fatal error during asset initialization", MB_OK | MB_ICONERROR);
	exit(EXIT_FAILURE);
}

// TODO: fix the godtier mess that is manual assetLoadError calls
static void loadRegistry() {
	if (!Registry::loadTexture("data/card_back.png", "card_back")) {
		assetLoadError("card_back", "data/card_back.png");
	}

	if (!Registry::loadTexture("data/splash.png", "splash")) {
		assetLoadError("splash", "data/splash.png");
	}
	if (!Registry::loadTexture("data/menu/bg.png", "bgmenu")) {
		assetLoadError("bgmenu", "data/menu/bg.png");
	}
	if (!Registry::loadTexture("data/settings/bg.png", "bgsettings")) {
		assetLoadError("bgsettings", "data/settings/bg.png");
	}

	if (!Registry::loadTexture("data/menu/btn.png", "menu_btn")) {
		assetLoadError("menu_btn", "data/menu/btn.png");
	}
	if (!Registry::loadTexture("data/settings/dropdown.png", "dropdown")) {
		assetLoadError("dropdown", "data/settings/dropdown.png");
	}
	if (!Registry::loadTexture("data/settings/category_btn_map.png", "category_btn_map")) {
		assetLoadError("category_btn_map", "data/settings/category_btn_map.png");
	}
	
	const auto cardFronts = Manifest::readManifest("./data/cards.txt");
	for (const auto& def : cardFronts) {
		if (!Registry::loadTexture("data/cards/" + def.path, def.id)) {
			assetLoadError(def.id, "data/cards/" + def.path);
		}
		loadedCards.push_back(def.id);
	}

	if (!Registry::loadSound("data/sound/matched.ogg", "matched")) {
		assetLoadError("matched", "data/sound/matched.ogg");
	}
	if (!Registry::loadSound("data/sound/max_win.ogg", "max_win")) {
		assetLoadError("max_win", "data/sound/max_win.ogg");
	}
	if (!Registry::loadSound("data/sound/hover.ogg", "hover")) {
		assetLoadError("hover", "data/sound/hover.ogg");
	}
	if (!Registry::loadSound("data/sound/splash.ogg", "splashfx")) {
		assetLoadError("splashfx", "data/sound/splash.ogg");
	}

	if (!Registry::loadMusic("data/sound/katamari.ogg", "katamari", "Fearofdark", "Rolling Down The Street, In My Katamari")) {
		assetLoadError("katamari", "data/sound/katamari.ogg");
	}

	if (!Registry::loadGlobalPosterizationShader("data/shader/posterize.frag")) {
		assetLoadError("globalPosterizationShader", "data/shader/posterize.frag");
	}

	if (!Registry::loadFont("data/medodica.otf")) {
		assetLoadError("font", "data/medodica.otf");
	}
}

static void loadScenery() {
	Scenery::registerFactory(SceneId::Game, []() { return std::make_unique<GameScene>(loadedCards); });
	Scenery::registerFactory(SceneId::Splash, []() { return std::make_unique<SplashScene>(); });
	Scenery::registerFactory(SceneId::Menu, []() { return std::make_unique<MenuScene>(); });
	Scenery::registerFactory(SceneId::Settings, []() { return std::make_unique<SettingsScene>(); });
}

int main(){
	loadRegistry();
	loadScenery();

	sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(Settings::VIRTUAL_WIDTH), static_cast<unsigned int>(Settings::VIRTUAL_HEIGHT) }), ":3");
	sf::RenderTexture renderTarget(window.getSize());
	sf::RectangleShape renderShape({ static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});

	Transition::init(window);
	Scenery::load(Settings::INITIAL_SCENE);

	sf::Clock deltaClock{};
	sf::Clock clock{};
	TimeSnap timeSnap{};
	while (window.isOpen()) {
		timeSnap.delta = deltaClock.restart().asSeconds();
		timeSnap.time = clock.getElapsedTime().asSeconds();

		while (const auto& ev = window.pollEvent()) {
			if (ev->is<sf::Event::Closed>()) {
				renderShape.setTexture(nullptr);
				window.close();
			}
			Scenery::active()->onSFMLEvent(ev);
		}

		BGM::update();
		Scenery::active()->update(renderTarget, window, timeSnap);
		Transition::update(timeSnap);

		renderTarget.clear(sf::Color::Black);
		Scenery::active()->draw(renderTarget, window, timeSnap);
		Transition::post_draw(renderTarget); // run post draw (transition)
		renderTarget.display();

		window.clear(sf::Color::Black);
		renderShape.setTexture(&renderTarget.getTexture());
		window.draw(renderShape, sf::RenderStates{&Registry::getGlobalPosterizationShader()});
		window.display();
	}
	Registry::deathAndDestruction();
}
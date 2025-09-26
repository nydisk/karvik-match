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

	Settings::MAIN_WINDOW = std::make_unique<sf::RenderWindow>(sf::VideoMode({ static_cast<unsigned int>(Settings::VIRTUAL_WIDTH), static_cast<unsigned int>(Settings::VIRTUAL_HEIGHT) }), ":3");
	Settings::MAIN_RENDERTARGET = std::make_unique<sf::RenderTexture>(Settings::MAIN_WINDOW->getSize());
	Settings::MAIN_RENDERSHAPE = std::make_unique<sf::RectangleShape>(sf::Vector2f{ static_cast<float>(Settings::MAIN_WINDOW->getSize().x), static_cast<float>(Settings::MAIN_WINDOW->getSize().y) });

	Transition::init(*Settings::MAIN_WINDOW);
	Scenery::load(Settings::INITIAL_SCENE);

	sf::Clock deltaClock{};
	sf::Clock clock{};
	TimeSnap timeSnap{};
	while (Settings::MAIN_WINDOW->isOpen()) {
		timeSnap.delta = deltaClock.restart().asSeconds();
		timeSnap.time = clock.getElapsedTime().asSeconds();

		while (const auto& ev = Settings::MAIN_WINDOW->pollEvent()) {
			if (ev->is<sf::Event::Closed>()) {
				Settings::MAIN_RENDERSHAPE->setTexture(nullptr);
				Settings::MAIN_WINDOW->close();
			}
			Scenery::active()->onSFMLEvent(ev);
		}

		BGM::update();
		Scenery::active()->update(*Settings::MAIN_RENDERTARGET, *Settings::MAIN_WINDOW, timeSnap);
		Transition::update(timeSnap);

		Settings::MAIN_RENDERTARGET->clear(sf::Color::Black);
		Scenery::active()->draw(*Settings::MAIN_RENDERTARGET, *Settings::MAIN_WINDOW, timeSnap);
		Transition::post_draw(*Settings::MAIN_RENDERTARGET); // run post draw (transition)
		Settings::MAIN_RENDERTARGET->display();

		Settings::MAIN_WINDOW->clear(sf::Color::Black);
		Settings::MAIN_RENDERSHAPE->setTexture(&(Settings::MAIN_RENDERTARGET->getTexture()));
		Settings::MAIN_WINDOW->draw(*Settings::MAIN_RENDERSHAPE, sf::RenderStates{&Registry::getGlobalPosterizationShader()});
		Settings::MAIN_WINDOW->display();
	}
	Registry::deathAndDestruction();
}
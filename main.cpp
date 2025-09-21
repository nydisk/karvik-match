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
#include "gameScene.hpp"
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

static void loadRegistry() {
	if (!Registry::loadTexture("./data/card_back.png", "card_back")) {
		assetLoadError("card_back", "/data/card_back.png");
	}
	
	const auto cardFronts = Manifest::readManifest("./data/cards.txt");
	for (const auto& def : cardFronts) {
		if (!Registry::loadTexture("./data/cards/" + def.path, def.id)) {
			assetLoadError(def.id, "/data/cards/" + def.path);
		}
		loadedCards.push_back(def.id);
	}

	if (!Registry::loadSound("./data/sound/matched.ogg", "matched")) {
		assetLoadError("matched", "/data/sound/matched.ogg");
	}
	if (!Registry::loadSound("./data/sound/max_win.ogg", "max_win")) {
		assetLoadError("max_win", "/data/sound/max_win.ogg");
	}
	if (!Registry::loadSound("./data/sound/hover.ogg", "hover")) {
		assetLoadError("hover", "/data/sound/hover.ogg");
	}

	if (!Registry::loadGlobalPosterizationShader("./data/posterize.frag")) {
		assetLoadError("globalPosterizationShader", "/data/posterize.frag");
	}
}

static void loadScenery() {
	Scenery::registerFactory(SceneId::Game, []() { return std::make_unique<GameScene>(loadedCards); });
}

int main(){
	loadRegistry();
	loadScenery();

	Scenery::load(SceneId::Game);

	sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(Settings::VIRTUAL_WIDTH), static_cast<unsigned int>(Settings::VIRTUAL_HEIGHT) }), ":3");
	sf::RenderTexture renderTarget(window.getSize());
	sf::RectangleShape renderShape({ static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});

	window.setSize({
		static_cast<unsigned int>(Settings::VIRTUAL_WIDTH * Settings::WINDOW_MULTIPLIER),
		static_cast<unsigned int>(Settings::VIRTUAL_HEIGHT * Settings::WINDOW_MULTIPLIER)
	});

	window.setPosition({
		static_cast<int>(sf::VideoMode::getDesktopMode().size.x / 2) - static_cast<int>(window.getSize().x / 2),
		static_cast<int>(sf::VideoMode::getDesktopMode().size.y / 2) - static_cast<int>(window.getSize().y / 2)
	});

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

		Scenery::active()->update(renderTarget, window, timeSnap);

		renderTarget.clear(sf::Color::Black);
		Scenery::active()->draw(renderTarget, window, timeSnap);
		renderTarget.display();

		window.clear(sf::Color::Black);
		renderShape.setTexture(&renderTarget.getTexture());
		window.draw(renderShape, sf::RenderStates{&Registry::getGlobalPosterizationShader()});
		window.display();
	}
	Registry::deathAndDestruction();
}
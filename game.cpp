#define _USE_MATH_DEFINES
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <random>
#include <string>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "registry.hpp"
#include "settings.hpp"
#include "revealer.hpp"
#include "timesnap.hpp"
#include "manifest.hpp"
#include "card.hpp"

std::random_device rd;
std::mt19937 gen(rd());

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
}
static void populateGameCards(std::vector<std::string>& target) {
	const int uniqueCards = (Card::CARDS_PER_COLUMN * Card::CARDS_PER_ROW) / 2;
	std::vector<std::string> copyOfCards = loadedCards;
	for (int i = 0; i < uniqueCards; i++) {
		std::uniform_int_distribution<int> dist{ 0,static_cast<int>(copyOfCards.size()) - 1 };
		size_t index = dist(gen);

		target.emplace_back(copyOfCards[index]);
		target.emplace_back(copyOfCards[index]);

		copyOfCards.erase(copyOfCards.begin() + index);
	}
	std::shuffle(target.begin(), target.end(), gen);
}

int main(){
	loadRegistry();

	sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(Settings::VIRTUAL_WIDTH), static_cast<unsigned int>(Settings::VIRTUAL_HEIGHT) }), ":3");
	
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

	std::vector<std::string> cardIds{};
	populateGameCards(cardIds);

	if (Card::CARDS_PER_COLUMN * Card::CARDS_PER_ROW != static_cast<int>(cardIds.size())) {
		std::cerr << "Card amount mismatch" << std::endl;
		std::cerr << " * expected: " << Card::CARDS_PER_COLUMN * Card::CARDS_PER_ROW << std::endl;
		std::cerr << " * got: " << cardIds.size() << std::endl;
		return -1;
	}

	Card* gameCards[Card::CARDS_PER_COLUMN][Card::CARDS_PER_ROW]{};
	for (int y = 0; y < Card::CARDS_PER_COLUMN; y++) {
		for (int x = 0; x < Card::CARDS_PER_ROW; x++) {
			gameCards[y][x] = new Card(cardIds[y * Card::CARDS_PER_ROW + x], {x,y});
		}
	}

	Revealer revealer{};
	TimeSnap timeSnap{};

	int givenPairs = (Card::CARDS_PER_COLUMN * Card::CARDS_PER_ROW) / 2;
	int mismatched = 0;

	while (window.isOpen()) {
		timeSnap.delta = deltaClock.restart().asSeconds();
		timeSnap.time = clock.getElapsedTime().asSeconds();

		while (const auto& ev = window.pollEvent()) {
			if (ev->is<sf::Event::Closed>()) window.close();
			if (const auto mbev = ev->getIf<sf::Event::MouseButtonPressed>()) {
				if (mbev->button != sf::Mouse::Button::Left || !window.hasFocus()) break;

				for (int y = 0; y < Card::CARDS_PER_COLUMN; y++) {
					for (int x = 0; x < Card::CARDS_PER_ROW; x++) {
						Card* c = gameCards[y][x];
						if (c == nullptr) continue;
						if (!c->isInteractable()) continue;
						if (!c->hovered()) continue;
						
						auto result = revealer.reveal(c);
						if (result == RevealResult::Mismatched) mismatched++;
						else if (result == RevealResult::Matched) SFX::play("matched");
					}
				}
			}
		}

		if (revealer.cardsRevealed() == Card::CARDS_PER_COLUMN * Card::CARDS_PER_ROW) {
			window.close();
			float rawEfficiency = (static_cast<float>(givenPairs) / static_cast<float>(givenPairs + mismatched)) * 100;
			float grynbergianEfficiency = ((static_cast<float>(givenPairs) * 1.75f) / static_cast<float>(givenPairs + mismatched)) * 100;
			
			std::ostringstream oss{};
			oss.precision(2);
			oss << "Raw Efficiency: " << std::fixed << rawEfficiency << "%\n"
				<< "Grynbergian Efficiency: " << std::fixed << grynbergianEfficiency << "%";


			SFX::play("max_win");

			MessageBoxA(nullptr, std::string(
				"MAX WIN!\n\nMinimal attempts: " + std::to_string(givenPairs) + " | " + std::to_string(static_cast<int>(static_cast<float>(givenPairs) * 1.75f)) +
				"\nAttempts: " + std::to_string(givenPairs + mismatched)
				+ "\n\n" + oss.str()
			).c_str(), "karvikmatch", MB_OK | MB_ICONINFORMATION);
		}
		
		for (int y = 0; y < Card::CARDS_PER_COLUMN; y++) {
			for (int x = 0; x < Card::CARDS_PER_ROW; x++) {
				if (gameCards[y][x] == nullptr) continue;
				gameCards[y][x]->update(window.mapPixelToCoords(sf::Mouse::getPosition(window)), timeSnap);
			}
		}

		window.clear(sf::Color::Black);

		for (int y = 0; y < Card::CARDS_PER_COLUMN; y++) {
			for (int x = 0; x < Card::CARDS_PER_ROW; x++) {
				if (gameCards[y][x] == nullptr) continue;
				gameCards[y][x]->draw(window);
			}
		}

		window.display();
	}

	// free the generated stuff
	for (int y = 0; y < Card::CARDS_PER_COLUMN; y++) {
		for (int x = 0; x < Card::CARDS_PER_ROW; x++) {
			delete gameCards[y][x];
		}
	}

}
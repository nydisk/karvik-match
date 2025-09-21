#pragma once
#include "scenery.hpp"
#include "revealer.hpp"
#include "scene.hpp"
#include "card.hpp"
#include "rng.hpp"

class GameScene : public Scene {
	std::vector<std::string> m_chosenCards{};
	Card* m_gameCards[Card::CARDS_PER_COLUMN][Card::CARDS_PER_ROW]{};
	Revealer m_revealer{};
	
	int m_mismatched{};
	
	bool m_gameOver = false;
	bool m_cardAnimationsFinished = false;
	float m_gameOverDelayTimer = 0;
	
	void populateGameCards(const std::vector<std::string>& loaded) {
		const int uniqueCards = (Card::CARDS_PER_COLUMN * Card::CARDS_PER_ROW) / 2;
		std::vector<std::string> copyOfCards = loaded;
		for (size_t i = 0; i < uniqueCards; i++) {
			size_t index = static_cast<size_t>(RNG::random(static_cast<int>(copyOfCards.size()) - 1));

			m_chosenCards.emplace_back(copyOfCards[index]);
			m_chosenCards.emplace_back(copyOfCards[index]);

			copyOfCards.erase(copyOfCards.begin() + index);
		}
		std::shuffle(m_chosenCards.begin(), m_chosenCards.end(), RNG::gen());
	}
	void handleClick() {
		for (int y = 0; y < Card::CARDS_PER_COLUMN; y++) {
			for (int x = 0; x < Card::CARDS_PER_ROW; x++) {
				Card* c = m_gameCards[y][x];
				if (c == nullptr) continue;
				if (!c->isInteractable()) continue;
				if (!c->hovered()) continue;

				auto result = m_revealer.reveal(c);
				if (result == RevealResult::Mismatched) m_mismatched++;
				else if (result == RevealResult::Matched) SFX::play("matched");
			}
		}
	}
	void triggerGameOver() {
		m_gameOver = true;
		m_gameOverDelayTimer = Card::CARD_TOTAL_DISAPPEAR_DELAY + Card::CARD_DISAPPEAR_TIME;
		m_cardAnimationsFinished = false;
	}
	void gameOver() const {
		constexpr int givenPairs = Card::CARDS_TOTAL / 2;
		float rawEfficiency = (static_cast<float>(givenPairs) / static_cast<float>(givenPairs + m_mismatched)) * 100;
		float grynbergianEfficiency = ((static_cast<float>(givenPairs) * 1.75f) / static_cast<float>(givenPairs + m_mismatched)) * 100;

		std::ostringstream oss{};

		oss.precision(2);
		oss << "Raw Efficiency: " << std::fixed << rawEfficiency << "%\n"
			<< "Grynbergian Efficiency: " << std::fixed << grynbergianEfficiency << "%";

		SFX::play("max_win");

		MessageBoxA(nullptr, std::string(
			"MAX WIN!\n\nMinimal attempts: " + std::to_string(givenPairs) + " | " + std::to_string(static_cast<int>(static_cast<float>(givenPairs) * 1.75f)) +
			"\nAttempts: " + std::to_string(givenPairs + m_mismatched)
			+ "\n\n" + oss.str()
		).c_str(), "karvikmatch", MB_OK | MB_ICONINFORMATION);

		Scenery::load(SceneId::Game); // automatically restart for now
	}
public:
	GameScene(const std::vector<std::string>& loadedCards) : Scene(SceneId::Game) {
		populateGameCards(loadedCards);
	}
	void update(sf::RenderTarget& renderTarget, sf::RenderWindow& window, const TimeSnap& time) override {
		if (m_revealer.cardsRevealed() == Card::CARDS_TOTAL && !m_gameOver) {
			triggerGameOver();
		}

		if (m_gameOver && !m_cardAnimationsFinished) {
			m_gameOverDelayTimer -= time.delta;
			if (m_gameOverDelayTimer <= 0.0F) {
				m_cardAnimationsFinished = true;
				gameOver();
			}
		}

		for (int y = 0; y < Card::CARDS_PER_COLUMN; y++) {
			for (int x = 0; x < Card::CARDS_PER_ROW; x++) {
				if (m_gameCards[y][x] == nullptr) continue;
				m_gameCards[y][x]->update(window.mapPixelToCoords(sf::Mouse::getPosition(window)), time);
			}
		}
	}
	void draw(sf::RenderTarget& renderTarget, sf::RenderWindow& window, const TimeSnap& time) override {
		for (int y = 0; y < Card::CARDS_PER_COLUMN; y++) {
			for (int x = 0; x < Card::CARDS_PER_ROW; x++) {
				if (m_gameCards[y][x] == nullptr) continue;
				m_gameCards[y][x]->draw(renderTarget);
			}
		}
	}
	void onLoad() override {
		if (Card::CARDS_TOTAL != static_cast<int>(m_chosenCards.size())) {
			std::cerr << "Card amount mismatch" << std::endl;
			std::cerr << " * expected: " << Card::CARDS_TOTAL << std::endl;
			std::cerr << " * got: " << m_chosenCards.size() << std::endl;
		}

		for (int y = 0; y < Card::CARDS_PER_COLUMN; y++) {
			for (int x = 0; x < Card::CARDS_PER_ROW; x++) {
				m_gameCards[y][x] = new Card(m_chosenCards[y * Card::CARDS_PER_ROW + x], { x,y });
			}
		}
	}
	void onUnload() override {
		for (int y = 0; y < Card::CARDS_PER_COLUMN; y++) {
			for (int x = 0; x < Card::CARDS_PER_ROW; x++) {
				delete m_gameCards[y][x];
			}
		}
	}
	void onSFMLEvent(const std::optional<sf::Event>& ev) override {
		if (m_gameOver) return;
		if (const auto mbev = ev->getIf<sf::Event::MouseButtonPressed>()) {
			if (mbev->button != sf::Mouse::Button::Left) return;

			handleClick();
		}
	}
};
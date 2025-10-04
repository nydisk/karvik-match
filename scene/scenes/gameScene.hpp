#pragma once
#include "scene/scenery.hpp"
#include "game/revealer.hpp"
#include "core/registry.hpp"
#include "core/settings.hpp"
#include "game/stats.hpp"
#include "scene/scene.hpp"
#include "game/card.hpp"
#include "util/rng.hpp"
#include "sound/bgm.hpp"

class GameScene : public Scene {
	std::vector<std::string> m_chosenCards{};
	Card* m_gameCards[Card::CARDS_PER_COLUMN][Card::CARDS_PER_ROW]{};
	Revealer m_revealer{};

	sf::Text m_nowPlayingText{Registry::getFont(), "nothing", Settings::SCALED_FONT_SIZE};
	
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
				if (result == RevealResult::Matched) SFX::play("matched");
			}
		}
	}
	void triggerGameOver() {
		m_gameOver = true;
		m_gameOverDelayTimer = Card::CARD_TOTAL_DISAPPEAR_DELAY + Card::CARD_DISAPPEAR_TIME;
		m_cardAnimationsFinished = false;
	}
	void gameOver() const {
		std::ostringstream oss{};

		oss.precision(2);
		oss << "Raw Efficiency: " << std::fixed << Statistics::rawEfficiency() << "%\n"
			<< "Grynbergian Efficiency (1.75x): " << std::fixed << Statistics::grynbergianEfficiency() << "%"
			<< "\nLucky guesses: " << Statistics::luckyGuesses() << "\n"
			<< "Repeated reveals: " << Statistics::monkeyBrain() << "\n"
			<< "\nFastest match: " << std::fixed << Statistics::fastest() << "s\n"
			<< "Slowest match: " << std::fixed << Statistics::slowest() << "s\n"
			<< "\nMinimum attempts: " << Statistics::minimalAttempts() << "\n"
			<< "Attempts: " << Statistics::attempts() << "\n"
			<< "Matches: " << Statistics::matches() << "\n"
			<< "Mismatches: " << Statistics::mismatches() << "\n\n"
			<< "\nGrade: " << Statistics::gradeString(Statistics::grade());

		SFX::play("max_win");
		std::cout << " = = STATISTICS = = " << std::endl;
		std::cout << oss.str() << std::endl;
		std::cout << " = = STATISTICS = = " << std::endl;

		int res = MessageBoxA(nullptr, oss.str().c_str(), "karvikmatch results", MB_RETRYCANCEL | MB_ICONINFORMATION);
		switch (res) {
		case IDRETRY:
			Scenery::load(SceneId::Game);	
			break;
		case IDABORT:
			Scenery::load(SceneId::Menu);
			break;
		default:
			Scenery::load(SceneId::Menu); // for now ig
			break;
		}
	}
public:
	GameScene(const std::vector<std::string>& loadedCards) : Scene(SceneId::Game) {
		populateGameCards(loadedCards);
	}
	void update(sf::RenderTarget& renderTarget, sf::RenderWindow& window, const TimeSnap& time) override {
		if (m_revealer.cardsRevealed() == Card::CARDS_TOTAL && !m_gameOver) {
			triggerGameOver();
		}

		RegistryMusicInfo* pMsc = BGM::currentlyPlaying();
		if (pMsc) {
			m_nowPlayingText.setString("now playing: \"" + pMsc->title + "\" - " + pMsc->author);
			m_nowPlayingText.setPosition({
				8,
				Settings::VIRTUAL_HEIGHT - m_nowPlayingText.getGlobalBounds().size.y - 8 - (m_nowPlayingText.getCharacterSize() / 2)
			});
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

		renderTarget.draw(m_nowPlayingText);
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

		BGM::queue("katamari");

		Statistics::reset();
		Scene::onLoad();
	}
	void onUnload() override {
		for (int y = 0; y < Card::CARDS_PER_COLUMN; y++) {
			for (int x = 0; x < Card::CARDS_PER_ROW; x++) {
				delete m_gameCards[y][x];
			}
		}
		Scene::onUnload();
	}
	void onSFMLEvent(const std::optional<sf::Event>& ev) override {
		if (m_gameOver) return;
		if (const auto mbev = ev->getIf<sf::Event::MouseButtonPressed>()) {
			if (mbev->button != sf::Mouse::Button::Left) return;

			handleClick();
		}
	}
};
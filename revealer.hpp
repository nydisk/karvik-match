#pragma once
#include <array>
#include "card.hpp"
#include "sfx.hpp"
#include "stats.hpp"
enum class RevealState {
	First,
	Second,
	Checking
};

enum class RevealResult {
	None,
	Matched,
	Mismatched
};

class Revealer {
	std::array<Card*, 2> m_revealed = { nullptr,nullptr };
	RevealState m_state = RevealState::First;
	int m_cardsRevealed = 0;

	void hideRevealed() {
		if (m_revealed[0]) m_revealed[0]->hide();
		if (m_revealed[1]) m_revealed[1]->hide();
		resetMemory();
	}
	void resetMemory() {
		m_revealed = { nullptr, nullptr };
	}
	[[nodiscard]] bool isRevealed(Card* c) {
		return c == m_revealed[0] || c == m_revealed[1];
	}
public:
	[[nodiscard]] int cardsRevealed() const {
		return m_cardsRevealed;
	}
	[[nodiscard]] RevealResult reveal(Card* c) {
		if (isRevealed(c)) return RevealResult::None; 

		Statistics::flipped(c);

		switch (m_state) {
		case RevealState::First:
			m_revealed[0] = c;
			c->show();
			m_state = RevealState::Second;
			return RevealResult::None;
		case RevealState::Second:
			m_revealed[1] = c;
			c->show();
			if (m_revealed[0]->is(m_revealed[1])) {
				m_revealed[0]->disappear();
				m_revealed[1]->disappear();
				Statistics::match(m_revealed[0], m_revealed[1]);

				m_cardsRevealed += 2;
				resetMemory();
				m_state = RevealState::First;


				return RevealResult::Matched;
			}
			else {
				Statistics::mismatch(m_revealed[0], m_revealed[1]);
				m_state = RevealState::Checking;
				return RevealResult::Mismatched;
			}
		case RevealState::Checking:
			hideRevealed();
			
			m_state = RevealState::Second;
			m_revealed[0] = c;
			c->show();

			return RevealResult::None;
		}

		return RevealResult::None;
	}
};
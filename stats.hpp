#pragma once
#include <array>
#include <SFML/System/Clock.hpp>
#include "card.hpp"

#undef max

class Statistics {
	inline static std::array<int, Card::CARDS_TOTAL> m_cardSeenCount{};

	inline static int m_totalPairs = Card::CARDS_TOTAL / 2;
	inline static int m_mismatched = 0;
	inline static int m_matched = 0;

	inline static int m_luckyGuesses = 0;
	inline static int m_monkeyBrain = 0;

	inline static sf::Clock m_matchTimer{};
	inline static float m_fastestMatch = std::numeric_limits<float>::max();
	inline static float m_slowestMatch = 0;

	inline static bool m_firstFlipDone = false;
public:
	inline static void reset() {
		m_firstFlipDone = false;
		m_cardSeenCount = std::array<int, Card::CARDS_TOTAL>{};
		m_totalPairs = Card::CARDS_TOTAL / 2;
		m_mismatched = 0;
		m_matched = 0;
		m_matchTimer = sf::Clock{};
		m_fastestMatch = std::numeric_limits<float>::max();
		m_slowestMatch = 0;
		m_luckyGuesses = 0;
	}
	inline static void mismatch(Card* card1, Card* card2) {
		m_mismatched++;

		if (m_cardSeenCount[card1->gridIndex()] > 1 || m_cardSeenCount[card2->gridIndex()] > 1) m_monkeyBrain++;
	}
	inline static void match(Card* card1, Card* card2) {
		m_matched++;
	
		float secondsSinceLastMatch = m_matchTimer.restart().asSeconds();
		if (secondsSinceLastMatch < m_fastestMatch)
			m_fastestMatch = secondsSinceLastMatch;
		if (secondsSinceLastMatch > m_slowestMatch)
			m_slowestMatch = secondsSinceLastMatch;

		if (m_cardSeenCount[card1->gridIndex()] == 1 && m_cardSeenCount[card2->gridIndex()] == 1) {
			m_luckyGuesses++;
		}
	}
	inline static void flipped(Card* card) {
		if (!m_firstFlipDone) { 
			m_firstFlipDone = true;
			m_matchTimer.restart();
		}
		m_cardSeenCount[card->gridIndex()]++;
	}
	inline static float grynbergianEfficiency() {
		return ((static_cast<float>(m_totalPairs) * 1.75f) / static_cast<float>(m_totalPairs + m_mismatched)) * 100;
	}
	inline static float rawEfficiency() {
		return (static_cast<float>(m_totalPairs) / static_cast<float>(m_totalPairs + m_mismatched)) * 100;
	}
	inline static float grade() {

	}
	inline static float slowest() {
		return m_slowestMatch;
	}
	inline static float fastest() {
		return m_fastestMatch;
	}
	inline static int attempts() {
		return m_matched + m_mismatched;
	}
	inline static int minimalAttempts() {
		return m_totalPairs;
	}
	inline static int luckyGuesses() {
		return m_luckyGuesses;
	}
	inline static int monkeyBrain() {
		return m_monkeyBrain;
	}
	inline static int mismatches() {
		return m_mismatched;
	}
	inline static int matches() {
		return m_matched;
	}
};
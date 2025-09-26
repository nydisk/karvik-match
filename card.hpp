#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "timesnap.hpp"
#include "settings.hpp"

enum class CardFlipState {
	FlippingNoFace,
	FlippingFace
};

class Card {
public:
	inline static const float CARD_SIZE_FACTOR = (Settings::VIRTUAL_HEIGHT / 512.0f); // relative to 512x512 | assume height priority
	inline static constexpr float CARD_WIDTH = 64;
	inline static const float REAL_WIDTH = CARD_SIZE_FACTOR * CARD_WIDTH;
	inline static constexpr float CARD_HEIGHT = 96;
	inline static const float REAL_HEIGHT = CARD_SIZE_FACTOR * CARD_HEIGHT;
	inline static constexpr float CARD_FLIP_TIME = 0.135f;
	inline static const float CARD_SEPARATION = 8 * CARD_SIZE_FACTOR;
	inline static constexpr float CARD_DISAPPEAR_TIME = 0.9f;
	inline static constexpr float CARD_DISAPPEAR_DELAY = 0.2f;
	inline static constexpr float CARD_HOVER_MULTIPLIER = 1.1f;
	inline static constexpr float CARD_TOTAL_DISAPPEAR_DELAY = CARD_FLIP_TIME + CARD_DISAPPEAR_DELAY;
	inline static constexpr int CARDS_PER_ROW = 4;
	inline static constexpr int CARDS_PER_COLUMN = 4;
	inline static constexpr int CARDS_TOTAL = CARDS_PER_COLUMN * CARDS_PER_ROW;
private:
	const sf::Texture& m_unflippedTex;
	const sf::Texture& m_flippedTex;
	
	std::string m_id;
	sf::Vector2f m_pos;
	sf::Vector2i m_gridPos;
	sf::RectangleShape m_shape;
	
	bool m_active = true;

	bool m_flipped = false;
	bool m_isInteractable = true;

	bool m_hovered = false;
	float m_hoverScale = 1.0f;

	CardFlipState m_flipAnimState;
	float m_flipTimer = 0.0f;
	float m_flipScale = 1.0f;
	bool m_flipIsSecondStage = true;

	float m_disappearTimer = 0.0f;
	bool m_shouldBeDisappearing = false;

	void resetFlipState();
	void updateTexture();
	void updateHoverScale(const TimeSnap& time);
	void updateFlip(const TimeSnap& time);
	[[nodiscard]] sf::FloatRect baseBounds() const;
	[[nodiscard]] static constexpr float totalWidth();
	[[nodiscard]] static constexpr float totalHeight();
	[[nodiscard]] static constexpr sf::Vector2f startingPosition();
	[[nodiscard]] static sf::Vector2f calculatePosition(const sf::Vector2i grid);
public:
	Card(const std::string& id, const sf::Vector2i& gridPos);
	void disappear();
	void show();
	void hide();
	void draw(sf::RenderTarget& target) const;
	void update(const sf::Vector2f& mousePos, const TimeSnap& time);
	[[nodiscard]] const sf::Vector2i& gridPos() const;
	[[nodiscard]] bool isInteractable() const;
	[[nodiscard]] bool is(const Card* other) const;
	[[nodiscard]] bool hovered() const;
	[[nodiscard]] int gridIndex() const;
};
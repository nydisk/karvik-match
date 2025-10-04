#include "sound/sfx.hpp"
#include "card.hpp"
#include "core/registry.hpp"
#include "core/settings.hpp"
#include "util/mathhelper.hpp"

[[nodiscard]] float Card::cardSizeFactor() { return Settings::VIRTUAL_HEIGHT / 512.0f; }
[[nodiscard]] float Card::realWidth() { return cardSizeFactor() * CARD_WIDTH; }
[[nodiscard]] float Card::realHeight() { return cardSizeFactor() * CARD_HEIGHT; }
[[nodiscard]] float Card::cardSeparation() { return 8.0f * cardSizeFactor(); }

float Card::totalWidth() {
	return (realWidth() * CARDS_PER_ROW) + ((CARDS_PER_ROW - 1) * cardSeparation());
}

float Card::totalHeight() {
	return (realHeight() * CARDS_PER_COLUMN) + ((CARDS_PER_COLUMN - 1) * cardSeparation());
}

sf::Vector2f Card::startingPosition() {
	return {
		(Settings::VIRTUAL_WIDTH - totalWidth()) / 2.0F,
		(Settings::VIRTUAL_HEIGHT - totalHeight()) / 2.0F
	};
}


sf::Vector2f Card::calculatePosition(const sf::Vector2i grid) {
	return startingPosition() + sf::Vector2f{
		static_cast<float>(grid.x) * (realWidth() + cardSeparation()) + (realWidth() / 2),
		static_cast<float>(grid.y) * (realHeight() + cardSeparation()) + (realHeight() / 2)
	};
}

void Card::updateTexture() {
	m_shape.setTexture((m_flipped ? &m_flippedTex : &m_unflippedTex), true);
}

Card::Card(const std::string& id, const sf::Vector2i& gridPos) : m_unflippedTex(Registry::getTexture("card_back")), m_flippedTex(Registry::getTexture(id)), m_id(id) {
	static_assert((CARDS_PER_ROW * CARDS_PER_COLUMN) % 2 == 0, "Total card count must be divisible by 2");

	m_gridPos = gridPos;

	m_shape = sf::RectangleShape({ realWidth(), realHeight() });
	m_shape.setOrigin({ realWidth() / 2, realHeight() / 2 });

	m_pos = Card::calculatePosition(gridPos);
	m_shape.setPosition(m_pos);

	updateTexture();
}

[[nodiscard]] const sf::Vector2i& Card::gridPos() const {
	return m_gridPos;
}

[[nodiscard]] int Card::gridIndex() const {
	return m_gridPos.x + Card::CARDS_PER_ROW * m_gridPos.y;
}

void Card::disappear() {
	m_isInteractable = false;
	m_shouldBeDisappearing = false;

	// account for incomplete flip time at the moment of call
	m_disappearTimer = CARD_TOTAL_DISAPPEAR_DELAY;
}

bool Card::isInteractable() const {
	return m_isInteractable;
}

bool Card::is(const Card* other) const {
	return other->m_id == m_id;
}

void Card::show() {
	m_flipped = true;
	m_flipAnimState = CardFlipState::FlippingNoFace;
	resetFlipState();
}

void Card::hide() {
	m_flipped = false;
	m_flipAnimState = CardFlipState::FlippingFace;
	resetFlipState();
}

void Card::resetFlipState() {
	m_flipIsSecondStage = false;
	m_flipTimer = 0.0F;
}

bool Card::hovered() const {
	return m_hovered;
}

void Card::draw(sf::RenderTarget& target) const {
	if (!m_active) return;
	target.draw(m_shape);
}

void Card::updateHoverScale(const TimeSnap& time){
	if (m_hovered) {
		m_hoverScale = CARD_HOVER_MULTIPLIER;
	}
	else {
		m_hoverScale = static_cast<float>(std::lerp(static_cast<double>(m_shape.getScale().y), 1.0, static_cast<double>(time.delta) * 12.5));
	}
	if (!m_isInteractable) m_hoverScale = 1.0f;
}

void Card::updateFlip(const TimeSnap& time) {
	m_flipTimer += time.delta;

	float t = std::clamp(m_flipTimer / CARD_FLIP_TIME, 0.0F, 1.0F);
	float st = math::smoothStep(t);
	m_flipScale = m_flipIsSecondStage ? st : 1 - st;

	switch (m_flipAnimState) {
	case CardFlipState::FlippingNoFace:
		if (t >= 1.0F && !m_flipIsSecondStage) {
			m_flipAnimState = CardFlipState::FlippingFace;
			m_flipIsSecondStage = true;
			m_flipTimer = 0;
			updateTexture();
		}
		break;
	case CardFlipState::FlippingFace:
		if (t >= 1.0F && !m_flipIsSecondStage) {
			m_flipAnimState = CardFlipState::FlippingNoFace;
			m_flipIsSecondStage = true;
			m_flipTimer = 0;
			updateTexture();
		}
		break;
	}
}
[[nodiscard]] sf::FloatRect Card::baseBounds() const {
	return {
	{m_pos.x - realWidth() / 2.0f,
	 m_pos.y - realHeight() / 2.0f},
	{realWidth(),
	 realHeight()}
	};
}
void Card::update(const sf::Vector2f& mousePos, const TimeSnap& time) {
	if (!m_active) return;

	if (!m_shouldBeDisappearing && !m_isInteractable) {
		if (m_disappearTimer > 0) {
			m_disappearTimer -= time.delta;
		}
		else if (m_disappearTimer < 0) {
			m_disappearTimer = CARD_DISAPPEAR_TIME;
			m_shouldBeDisappearing = true; // delay has finished yay!
		}
	}

	if (m_shouldBeDisappearing && !m_isInteractable) {
		if (m_disappearTimer <= 0) { 
			m_active = false;
			hide();
		}
		else {
			m_disappearTimer -= time.delta;
		}

		float dprog = std::clamp(m_disappearTimer / CARD_DISAPPEAR_TIME, 0.0F, 1.0F);
		dprog = math::smoothStep(dprog);

		m_shape.setScale({
			dprog,
			dprog
		});
	}

	bool hoveringRightNow = baseBounds().contains(mousePos);
	if (!m_hovered && hoveringRightNow && m_flipTimer >= 1.0f) {
		SFX::play("hover");
	}
	m_hovered = hoveringRightNow;
	updateHoverScale(time);
	updateFlip(time);

	if (!m_shouldBeDisappearing) {
		m_shape.setScale({
			m_flipScale * m_hoverScale,
			m_hoverScale
		});
	}
}

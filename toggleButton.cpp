#include "toggleButton.hpp"

ToggleButton::ToggleButton(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& idleColor, const sf::Color& hoverColor, const sf::Color& activeColor, const std::string& text, const std::string& texture, const unsigned int minCharSize, const unsigned int maxCharSize)
	: Button(size, position, text, idleColor, hoverColor, activeColor, texture, minCharSize, maxCharSize)
{
	setUseHoverImage(false);
}

void ToggleButton::update(const sf::Vector2f& mousePos) {
	Button::update(mousePos);

	if (wasClicked()) {
		m_toggled = !m_toggled;
	}

	if (isHovered()) {
		getShape().setFillColor(isPressed() ? m_activeColor : m_hoverColor);
	}
	else {
		getShape().setFillColor(m_idleColor);
	}

	if (!hasTexture()) return;
	getShape().setTextureRect(m_toggled ? getActiveRect() : getIdleRect());
}

bool ToggleButton::toggled() const { return m_toggled; }
void ToggleButton::setToggled(bool state) { m_toggled = state; }

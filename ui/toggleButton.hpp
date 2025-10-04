#pragma once
#include "button.hpp"

class ToggleButton : public Button {
	bool m_toggled = false;
public:
	ToggleButton(const sf::Vector2f& size, const sf::Vector2f& position,
		const sf::Color& idleColor, const sf::Color& hoverColor, const sf::Color& activeColor,
		const std::string& text = "",
		const std::string& texture = "",
		const unsigned int minCharSize = 0, const unsigned int maxCharSize = 24u);

	void update(const sf::Vector2f& mousePos);

	bool toggled() const;
	void setToggled(bool state);
};
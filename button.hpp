#pragma once
#include <SFML/Graphics.hpp>
#include "registry.hpp"
class Button {
	sf::RectangleShape m_shape;
	sf::Text m_text;
	sf::Color m_idleColor;
	sf::Color m_hoverColor;
	sf::Color m_activeColor;
	bool m_isPressed = false;
	bool m_isHovered = false;
	bool m_wasHovered = false;
	bool m_wasActivated = false;
public:
	Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text,
		const sf::Color& idleColor, const sf::Color& hoverColor, const sf::Color& activeColor, const unsigned int minCharSize = 0, const unsigned int maxCharSize = 16u);
	void update(const sf::Vector2f& mousePos);
	void draw(sf::RenderTarget& target) const;
	bool isPressed() const;
	bool isHovered() const;
	bool wasClicked() const;
	bool wasHovered() const;
};
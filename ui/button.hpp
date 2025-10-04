#pragma once
#include <SFML/Graphics.hpp>
#include "core/registry.hpp"
class Button {
	sf::RectangleShape m_shape;
	sf::Text m_text;
protected:
	sf::Color m_idleColor;
	sf::Color m_hoverColor;
	sf::Color m_activeColor;
private:

	const sf::Texture* m_texture;

	bool m_isPressed = false;
	bool m_isHovered = false;
	bool m_wasHovered = false;
	bool m_wasActivated = false;

	bool m_useHoverImage = true;

	sf::IntRect m_idleRect{};
	sf::IntRect m_hoverRect{};
	sf::IntRect m_activeRect{};
protected:
	bool hasTexture() const;
	const sf::IntRect& getActiveRect() const;
	const sf::IntRect& getIdleRect() const;
public:
	Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text,
		const sf::Color& idleColor, const sf::Color& hoverColor, const sf::Color& activeColor,
		const std::string& texture = "",
		const unsigned int minCharSize = 0, const unsigned int maxCharSize = 24u);
	void recalculateRects();
	void forceRects(const sf::IntRect& idle, const sf::IntRect& hover, const sf::IntRect& active);
	void update(const sf::Vector2f& mousePos);
	void draw(sf::RenderTarget& target) const;
	void setUseHoverImage(const bool useHoverImage);
	const sf::RectangleShape& getShape() const;
	sf::RectangleShape& getShape();
	void setPosition(const sf::Vector2f& position);
	[[nodiscard]] bool usesHoverImage() const;
	[[nodiscard]] bool isPressed() const;
	[[nodiscard]] bool isHovered() const;
	[[nodiscard]] bool wasClicked() const;
	[[nodiscard]] bool wasHovered() const;
};
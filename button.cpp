#include "button.hpp"

Button::Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text, const sf::Color& idleColor, const sf::Color& hoverColor, const sf::Color& activeColor, const std::string& texture, const unsigned int minCharSize, const unsigned int maxCharSize)
	: m_idleColor(idleColor), m_hoverColor(hoverColor), m_activeColor(activeColor),
	  m_texture(texture == "" ? nullptr : &Registry::getTexture(texture)),
	  m_text(Registry::getFont())
{
	m_shape.setSize(size);
	m_shape.setPosition(position);
	m_shape.setFillColor(m_idleColor);

	m_text.setString(text);
	m_text.setCharacterSize(std::clamp(static_cast<unsigned int>(size.y * 0.5f), minCharSize, maxCharSize));
	m_text.setFillColor(sf::Color::White);

	sf::FloatRect textBounds = m_text.getLocalBounds();
	m_text.setOrigin({ textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f });
	m_text.setPosition({ position.x + size.x / 2.0f, position.y + size.y / 2.0f });

	if (m_texture) {
		m_shape.setTexture(m_texture);
		recalculateRects();
	}
}

void Button::recalculateRects() {
	if (!m_texture) return;
	const sf::Vector2u texSize = m_texture->getSize();
	if (texSize.y % 3 != 0) {
		std::cerr << "warning: button texture has to be divisible by 3 (safe to ignore if using Button::forceRects):\n * " << texSize.y << " is not divisible by 3" << std::endl;
		m_texture = nullptr;
		return;
	}
	m_idleRect = { {0, 0}, {static_cast<int>(texSize.x), static_cast<int>(texSize.y / 3)} };
	m_hoverRect = { {0, static_cast<int>(texSize.y / 3)}, {static_cast<int>(texSize.x), static_cast<int>(texSize.y / 3)} };
	m_activeRect = { {0, static_cast<int>(texSize.y * 2 / 3)}, {static_cast<int>(texSize.x), static_cast<int>(texSize.y / 3)} };

	// restore default state
	m_shape.setTextureRect(m_idleRect);
}

void Button::forceRects(const sf::IntRect& idle, const sf::IntRect& hover, const sf::IntRect& active) {
	if (!m_texture) return;
	m_idleRect = idle;
	m_hoverRect = hover;
	m_activeRect = active;

	// restore default state
	m_shape.setTextureRect(m_idleRect);
}

void Button::update(const sf::Vector2f& mousePos) {
	m_wasActivated = false;
	m_wasHovered = false;
	bool hov = m_shape.getGlobalBounds().contains(mousePos);
	if (hov && !m_isHovered) {
		m_wasHovered = true;
	}
	m_isHovered = hov;
	if (m_isHovered) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			m_isPressed = true;
			m_shape.setFillColor(m_activeColor);
			if (m_texture) {
				m_shape.setTextureRect(m_activeRect);
			}
		}
		else {
			if (m_isPressed) {
				m_wasActivated = true;
				m_isPressed = false;
			}
			m_shape.setFillColor(m_hoverColor);
			if (m_texture) {
				m_shape.setTextureRect(m_hoverRect);
			}
		}
	}
	else {
		m_isPressed = false;
		m_shape.setFillColor(m_idleColor);
		if (m_texture) {
			m_shape.setTextureRect(m_idleRect);
		}
	}
}

void Button::draw(sf::RenderTarget& target) const {
	target.draw(m_shape);
	target.draw(m_text);
}

bool Button::isPressed() const { return m_isPressed; }
bool Button::isHovered() const { return m_isHovered; }
bool Button::wasClicked() const { return m_wasActivated; }
bool Button::wasHovered() const { return m_wasHovered; }

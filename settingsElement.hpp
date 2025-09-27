#pragma once
#include <functional>
#include <SFML/Graphics.hpp>
#include "settings.hpp"
#include "registry.hpp"

class SettingsElement {
public:
	enum class ElementType {
		Dropdown,
		Other
	};
protected:
	inline static float PADDING = 8.0F * (Settings::VIRTUAL_WIDTH / 1280.0F);
	std::string m_displayText;
	sf::RectangleShape m_optionBackground;
	sf::Text m_optionText{ Registry::getFont(), m_displayText, Settings::SCALED_FONT_SIZE };
	std::string m_identifier;
	std::function<void(SettingsElement&)> m_onChangeCallback = nullptr;
	ElementType m_type;
public:
	SettingsElement(const std::string& id, const std::string& displayText, const sf::Vector2f& size, const sf::Vector2f& position);
	virtual void update(const sf::Vector2f& mousePos) = 0;
	virtual void draw(sf::RenderTarget& target);
	[[nodiscard]] const std::string& id() const;
	[[nodiscard]] ElementType type() const;
	void setChangeCallback(const std::function<void(SettingsElement&)>& callback);
};
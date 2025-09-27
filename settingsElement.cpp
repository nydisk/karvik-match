#include "settingsElement.hpp"

SettingsElement::SettingsElement(const std::string& id, const std::string& displayText, const sf::Vector2f& size, const sf::Vector2f& position) :
	m_identifier(id),
	m_displayText(displayText),
	m_optionBackground(size),
	m_optionText(Registry::getFont(), m_displayText, Settings::SCALED_FONT_SIZE),
	m_type(ElementType::Other)
{
	m_optionBackground.setPosition(position);
	m_optionBackground.setFillColor({ 45,45,45,255 });
	m_optionBackground.setOutlineColor({ 255,255,255,69 });
	m_optionBackground.setOutlineThickness(-2.0F);

	m_optionText.setPosition({ position.x + PADDING, position.y + (size.y / 2) - (m_optionText.getGlobalBounds().size.y / 2) - (m_optionText.getCharacterSize() / 2) });
}

void SettingsElement::draw(sf::RenderTarget& target) {
	target.draw(m_optionBackground);
	target.draw(m_optionText);
}

[[nodiscard]] const std::string& SettingsElement::id() const { return m_identifier; }

[[nodiscard]] SettingsElement::ElementType SettingsElement::type() const { return m_type; }

void SettingsElement::setChangeCallback(const std::function<void(SettingsElement&)>& callback) { m_onChangeCallback = callback; }

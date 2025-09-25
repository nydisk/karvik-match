#pragma once
#include "scene.hpp"
#include "scenery.hpp"
#include "registry.hpp"
#include "sfx.hpp"
#include "button.hpp"

class MenuScene : public Scene{
public:
	static constexpr float BUTTON_PADDING = 8.0F;
	static constexpr float BUTTON_COUNT = 3.0F;
	static constexpr float TOTAL_PADDING = BUTTON_PADDING * (BUTTON_COUNT + 1);
	static constexpr sf::Color BUTTON_IDLE_COLOR{ 0,0,0,128 };
	static constexpr sf::Color BUTTON_HOVER_COLOR{ 48,48,48,200 };
	static constexpr sf::Color BUTTON_ACTIVE_COLOR{ 0,0,0,255 };
	static constexpr unsigned int BUTTON_MIN_CHAR_SIZE = 12u;
	static constexpr unsigned int BUTTON_MAX_CHAR_SIZE = 16u;
private:
	enum class MenuOption : size_t {
		Play,
		Settings,
		Exit
	};
	struct MenuButton {
		Button button;
		MenuOption id;
	};

	sf::Text m_versionText{ Registry::getFont(), Settings::VERSION_STRING, 12u };
	sf::RectangleShape m_background{ {Settings::VIRTUAL_WIDTH, Settings::VIRTUAL_HEIGHT} };
	sf::RectangleShape m_choicesBackground{ {Settings::VIRTUAL_WIDTH * 0.6F, Settings::VIRTUAL_HEIGHT * 0.3F} };
	std::vector<MenuButton> m_buttons{};
	bool m_userMadeChoice = false;

	void createButton(const std::string& text, const MenuOption idx) {
		const sf::Vector2f buttonSize{ m_choicesBackground.getSize().x - BUTTON_PADDING * 2, (m_choicesBackground.getSize().y - TOTAL_PADDING) / BUTTON_COUNT };
		Button btn{
			buttonSize,
			{
				m_choicesBackground.getPosition().x + BUTTON_PADDING,
				m_choicesBackground.getPosition().y + BUTTON_PADDING + (buttonSize.y + BUTTON_PADDING) * static_cast<float>(idx)
			},
			text,
			BUTTON_IDLE_COLOR,
			BUTTON_HOVER_COLOR,
			BUTTON_ACTIVE_COLOR,
			BUTTON_MIN_CHAR_SIZE,
			BUTTON_MAX_CHAR_SIZE
		};
		m_buttons.emplace_back(btn, idx);
	}
public:
	MenuScene() : Scene(SceneId::Menu) {}
	void update(sf::RenderTarget& target, sf::RenderWindow& window, const TimeSnap& time) override {
		for (auto& btn : m_buttons) {
			btn.button.update(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
			if (m_userMadeChoice) continue;
			if (btn.button.wasHovered()) {
				SFX::play("hover");
			}
			if (!btn.button.wasClicked()) continue;
			m_userMadeChoice = true;
			switch (btn.id) {
				case MenuOption::Play:
					Scenery::load(SceneId::Game);
					break;
				case MenuOption::Settings:
					//Scenery::load(SceneId::Settings);
					break;
				case MenuOption::Exit:
					window.close();
					break;
			}
		}
	}
	void draw(sf::RenderTarget& target, sf::RenderWindow& window, const TimeSnap& time) override {
		target.draw(m_background);
		target.draw(m_versionText);
		target.draw(m_choicesBackground);
		for (const auto& btn : m_buttons) {
			btn.button.draw(target);
		}
	}
	void onSFMLEvent(const std::optional<sf::Event>& ev) override {
		
	}
	void onUnload() override {

		Scene::onUnload();
	}
	void onLoad() override {
		m_background.setTexture(&Registry::getTexture("bgmenu"));
		m_versionText.setPosition({
			Settings::VIRTUAL_WIDTH - m_versionText.getGlobalBounds().size.x - 8,
			Settings::VIRTUAL_WIDTH - m_versionText.getGlobalBounds().size.y - 8
		});
		
		m_choicesBackground.setFillColor(sf::Color(0, 0, 0, 100));
		m_choicesBackground.setPosition({
			(Settings::VIRTUAL_WIDTH - m_choicesBackground.getSize().x) / 2.0F,
			(Settings::VIRTUAL_HEIGHT - m_choicesBackground.getSize().y) / 2.0F + Settings::VIRTUAL_HEIGHT * 0.25F
		});
		
		createButton("play", MenuOption::Play);
		createButton("settings", MenuOption::Settings);
		createButton("exit to windows", MenuOption::Exit);

		Scene::onLoad();
	}
};
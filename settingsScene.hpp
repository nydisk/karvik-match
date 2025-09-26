#pragma once
#include "sfx.hpp"
#include "scene.hpp"
#include "scenery.hpp"
#include "registry.hpp"
#include "toggleButton.hpp"
#include "settings.hpp"
#include <SFML/Graphics.hpp>

class SettingsScene : public Scene {
	inline static constexpr float CATEGORY_BUTTON_SCALE_FACTOR = Settings::VIRTUAL_WIDTH / 1280.0F;
	inline static constexpr float CATEGORY_BUTTON_PADDING = 8.0F;

	inline static constexpr sf::Vector2f CATEGORY_BUTTON_SIZE = { 48.0F,48.0F };
	inline static constexpr sf::Vector2i CATEGORY_BUTTON_TEXTURE_SIZE = { 15,15 };
	
	inline static constexpr float CATEGORY_BUTTON_ADJUSTED_PADDING = CATEGORY_BUTTON_PADDING * CATEGORY_BUTTON_SCALE_FACTOR;
	inline static constexpr sf::Vector2f CATEGORY_BUTTON_ADJUSTED_SIZE = { CATEGORY_BUTTON_SIZE.x * CATEGORY_BUTTON_SCALE_FACTOR, CATEGORY_BUTTON_SIZE.y * CATEGORY_BUTTON_SCALE_FACTOR };
	
	inline static constexpr sf::Color IDLE_BUTTON_COLOR{ 255,255,255,255 };
	inline static constexpr sf::Color HOVER_BUTTON_COLOR{ 200,200,200,255 };
	inline static constexpr sf::Color ACTIVE_BUTTON_COLOR{ 150,150,150,255 };

	enum class SettingsCategoryId : size_t {
		Graphics,
		Audio,
		Misc
	};
	struct CategoryButton {
		SettingsCategoryId id;
		ToggleButton button;
	};

	std::vector<CategoryButton> m_categories{};
	SettingsCategoryId m_currentCategory = SettingsCategoryId::Graphics;
	sf::RectangleShape m_categoriesBackground{ {CATEGORY_BUTTON_ADJUSTED_SIZE.x + (CATEGORY_BUTTON_ADJUSTED_PADDING * 2), Settings::VIRTUAL_HEIGHT} };

	Button m_backButton;
	bool m_returningToMenu = false;

	void addCategoryButton(const SettingsCategoryId id) {
		ToggleButton btn{
			CATEGORY_BUTTON_ADJUSTED_SIZE,
			{
				CATEGORY_BUTTON_ADJUSTED_PADDING,
				CATEGORY_BUTTON_ADJUSTED_PADDING + (CATEGORY_BUTTON_ADJUSTED_PADDING + CATEGORY_BUTTON_ADJUSTED_SIZE.y) * static_cast<float>(m_categories.size())
			},
			IDLE_BUTTON_COLOR,
			HOVER_BUTTON_COLOR,
			ACTIVE_BUTTON_COLOR,
			"",
			"category_btn_map"
		};

		btn.forceRects(
			{ {static_cast<int>(CATEGORY_BUTTON_TEXTURE_SIZE.x * static_cast<int>(id)), 0}, {CATEGORY_BUTTON_TEXTURE_SIZE.x, CATEGORY_BUTTON_TEXTURE_SIZE.y} },
			{},
			{ {static_cast<int>(CATEGORY_BUTTON_TEXTURE_SIZE.x * static_cast<int>(id)), CATEGORY_BUTTON_TEXTURE_SIZE.y}, {CATEGORY_BUTTON_TEXTURE_SIZE.x, CATEGORY_BUTTON_TEXTURE_SIZE.y} }
		);

		m_categories.emplace_back(id,btn);
	}

	void switchCategory(const SettingsCategoryId id) {
		m_currentCategory = id;
		for(auto& cat : m_categories) {
			cat.button.setToggled(cat.id == id);
		}
	}
public:
	SettingsScene() : Scene(SceneId::Settings),
		m_backButton(
			CATEGORY_BUTTON_SIZE,
			{CATEGORY_BUTTON_ADJUSTED_PADDING, Settings::VIRTUAL_HEIGHT - CATEGORY_BUTTON_ADJUSTED_PADDING - CATEGORY_BUTTON_ADJUSTED_SIZE.y},
			"",
			IDLE_BUTTON_COLOR,
			HOVER_BUTTON_COLOR,
			ACTIVE_BUTTON_COLOR,
			"category_btn_map")
	{
		m_backButton.forceRects(
			{ {static_cast<int>(CATEGORY_BUTTON_TEXTURE_SIZE.x * 3), 0}, {CATEGORY_BUTTON_TEXTURE_SIZE.x, CATEGORY_BUTTON_TEXTURE_SIZE.y} },
			{ {static_cast<int>(CATEGORY_BUTTON_TEXTURE_SIZE.x * 3), CATEGORY_BUTTON_TEXTURE_SIZE.y}, {CATEGORY_BUTTON_TEXTURE_SIZE.x, CATEGORY_BUTTON_TEXTURE_SIZE.y} },
			{ {static_cast<int>(CATEGORY_BUTTON_TEXTURE_SIZE.x * 3), CATEGORY_BUTTON_TEXTURE_SIZE.y}, {CATEGORY_BUTTON_TEXTURE_SIZE.x, CATEGORY_BUTTON_TEXTURE_SIZE.y} }
		);
	}
	void update(sf::RenderTarget& target, sf::RenderWindow& window, const TimeSnap& time) override {
		for(auto& cat : m_categories) {
			cat.button.update(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
			if (!cat.button.wasClicked()) continue;
			if (cat.id == m_currentCategory) continue;
			switchCategory(cat.id);
		}
		m_backButton.update(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		if (m_backButton.wasClicked() && !m_returningToMenu) {
			Scenery::load(SceneId::Menu);
			m_returningToMenu = true;
		}
	}
	void draw(sf::RenderTarget& target, sf::RenderWindow& window, const TimeSnap& time) override {
		target.draw(m_categoriesBackground);	
		for(const auto& cat : m_categories) {
			cat.button.draw(target);
		}
		m_backButton.draw(target);
	}
	void onLoad() override {
		addCategoryButton(SettingsCategoryId::Graphics);
		addCategoryButton(SettingsCategoryId::Audio);
		addCategoryButton(SettingsCategoryId::Misc);
		switchCategory(SettingsCategoryId::Graphics);

		m_categoriesBackground.setPosition({ 0.0F,0.0F });
		m_categoriesBackground.setFillColor({ 35,35,35,255 });
		m_categoriesBackground.setOutlineColor({ 75,75,75,255 });
		m_categoriesBackground.setOutlineThickness(-2.0F);

		Scene::onLoad();
	}
	void onUnload() override { Scene::onUnload(); }
	void onSFMLEvent(const std::optional<sf::Event>& ev) override {}
};
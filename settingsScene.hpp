#pragma once
#include <type_traits>
#include <functional>
#include <SFML/Graphics.hpp>

#include "sfx.hpp"
#include "scene.hpp"
#include "scenery.hpp"
#include "registry.hpp"
#include "toggleButton.hpp"
#include "settings.hpp"
#include "config.hpp"

#include "settingsElement.hpp"

#undef max
#undef min

class DropdownSetting : public SettingsElement {
	std::vector<Button> m_optionButtons{};

	sf::RectangleShape m_dropdownArea{ {} };
	sf::RectangleShape m_dropdownChoicesArea{ {} };
	sf::RectangleShape m_scrollBar{ {} };

	Button m_dropdownArrow;
	sf::Text m_selectedOptionText{ Registry::getFont(), "Selected Option", Settings::SCALED_FONT_SIZE };

	bool m_expanded = false;
	bool m_justExpanded = false;
	std::vector<std::string> m_options{};
	size_t m_selectedOption = 0;
	
	int m_visibleOptions = 5;
	int m_scrollOffset = 0;

	void updateScrollOffset(const int difference) {
		m_scrollOffset = std::clamp(m_scrollOffset + difference, 0, std::max(0, static_cast<int>(m_options.size()) - m_visibleOptions));
	}
public:
	DropdownSetting(const std::string& id, const std::string& displayText, const sf::Vector2f& size, const sf::Vector2f& position, const std::vector<std::string>& options, const size_t defaultOption = 0) :
		SettingsElement(id, displayText, size, position),
		m_dropdownArrow(
			{ size.y - (PADDING * 2), size.y - (PADDING * 2) },
			{ position.x + size.x - (size.y - (PADDING * 2)) - PADDING, position.y + PADDING },
			"",
			{ 255,255,255,255 },
			{ 200,200,200,255 },
			{ 150,150,150,255 },
			"dropdown"
		),
		m_selectedOption(defaultOption)
	{
		m_options = options;

		m_dropdownArrow.forceRects(
			{ {0,0}, {16,16} },
			{ {0,0}, {16,16} },
			{ {0,0}, {16,16} }
		);

		float longestOptionWidth = 0.0F;
		for(const auto& opt : m_options) {
			sf::Text tempText{ Registry::getFont(), opt, Settings::SCALED_FONT_SIZE };
			if (tempText.getLocalBounds().size.x > longestOptionWidth) {
				longestOptionWidth = tempText.getLocalBounds().size.x;
			}
		}

		const float arrowSize = size.y - (PADDING * 2);

		m_dropdownArea.setSize({ longestOptionWidth + (PADDING * 2) + (arrowSize * 2) + PADDING, size.y - (PADDING * 2)});
		m_dropdownArea.setPosition({ position.x + size.x - PADDING - (m_dropdownArea.getSize().x), position.y + PADDING});
		m_dropdownArea.setFillColor({ 25,25,25,255 });
		m_dropdownArea.setOutlineColor({ 255,255,255,69 });
		m_dropdownArea.setOutlineThickness(-2.0F);

		m_dropdownChoicesArea.setFillColor({ 25,25,25,255 });
		m_dropdownChoicesArea.setOutlineColor({ 255,255,255,69 });
		m_dropdownChoicesArea.setOutlineThickness(-2.0F);

		const float choiceHeight = size.y - (PADDING * 2);
		const float outerPadding = 4.f;

		m_dropdownChoicesArea.setSize({ m_dropdownArea.getSize().x, choiceHeight * static_cast<float>(m_visibleOptions) + (outerPadding * 2) });
		m_dropdownChoicesArea.setPosition({ m_dropdownArea.getPosition().x, m_dropdownArea.getPosition().y + m_dropdownArea.getSize().y });

		for (size_t i = 0; i < m_options.size(); ++i) {
			sf::Vector2f btnSize = {
				m_dropdownChoicesArea.getSize().x - (PADDING * 2),
				choiceHeight - PADDING
			};
			sf::Vector2f btnPos = {
				m_dropdownChoicesArea.getPosition().x + PADDING,
				m_dropdownChoicesArea.getPosition().y + outerPadding + i * choiceHeight + (PADDING / 2.f)
			};
			Button optBtn{
				btnSize,
				btnPos,
				m_options[i],
				{ 45,45,45,255 },
				{ 100,100,100,255 },
				{ 150,150,150,255 },
				"",
				0U,
				static_cast<unsigned int>(choiceHeight)
			};
			m_optionButtons.push_back(optBtn);
		}

		m_selectedOptionText.setCharacterSize(static_cast<unsigned int>(Settings::SCALED_FONT_SIZE));
		m_selectedOptionText.setString(m_options.empty() ? "N/A" : m_options[m_selectedOption]);
		m_selectedOptionText.setPosition({ m_dropdownArea.getPosition().x + PADDING, m_dropdownArea.getPosition().y + (m_dropdownArea.getSize().y / 2) - (m_selectedOptionText.getGlobalBounds().size.y / 2) - (m_selectedOptionText.getCharacterSize() / 2) });
	
		m_type = ElementType::Dropdown;

		const float fullScrollBarHeight = m_dropdownChoicesArea.getSize().y - (outerPadding * 2);
		const float optionsRatio = static_cast<float>(m_visibleOptions) / static_cast<float>(m_options.size());
		float scrollHeight = (optionsRatio * fullScrollBarHeight);

		m_scrollBar = sf::RectangleShape{ {outerPadding / 2, scrollHeight} };
		m_scrollBar.setPosition({
			m_dropdownChoicesArea.getPosition().x + m_dropdownChoicesArea.getSize().x - m_scrollBar.getSize().x - outerPadding,
			m_dropdownChoicesArea.getPosition().y + outerPadding
		});
		m_scrollBar.setFillColor({ 255,255,255,255 });
	}
	void update(const sf::Vector2f& mousePos) override {
		m_justExpanded = false;
		m_dropdownArrow.update(mousePos);
		if (m_dropdownArrow.wasClicked()) {
			m_expanded = !m_expanded;
			if (m_expanded) {
				m_justExpanded = true;
			}
		}
		if (m_expanded) {
			for (size_t i = 0; i < m_optionButtons.size(); ++i) {
				if (i < static_cast<size_t>(m_scrollOffset) || i >= static_cast<size_t>(m_scrollOffset + m_visibleOptions)) continue;

				m_optionButtons[i].update(mousePos);
				if (!m_optionButtons[i].wasClicked()) continue;
				m_selectedOption = i;
				m_selectedOptionText.setString(m_options[m_selectedOption]);
				
				if (m_onChangeCallback) m_onChangeCallback(*this);

				m_expanded = false;
			}
		}
	}
	void draw(sf::RenderTarget& target) override {
		SettingsElement::draw(target);
		
		target.draw(m_dropdownArea);
		target.draw(m_selectedOptionText);
		
		m_dropdownArrow.draw(target);
	}
	void postDraw(sf::RenderTarget& target) {
		if (m_expanded) {
			target.draw(m_dropdownChoicesArea);
			for (size_t i = 0; i < m_optionButtons.size(); ++i) {
				if (i < static_cast<size_t>(m_scrollOffset) || i >= static_cast<size_t>(m_scrollOffset + m_visibleOptions)) continue;

				m_optionButtons[i].draw(target);
			}
			if (m_options.size() > static_cast<size_t>(m_visibleOptions)) {
				target.draw(m_scrollBar);
			}
		}
	}
	bool isExpanded() const { return m_expanded; }
	bool justExpanded() const { return m_justExpanded; }
	size_t getSelectedOption() const { return m_selectedOption; }
	const std::string& getSelectedOptionString() const { return m_options[m_selectedOption]; }

	void forceExpanded(const bool expanded) {
		m_expanded = expanded;
	}

	void scroll(const int direction) {
		const float choiceHeight = m_optionBackground.getSize().y - (PADDING * 2);
		const float outerPadding = 4.f;
		updateScrollOffset(direction);
		// update all button positions
		for(size_t i = 0; i < m_optionButtons.size(); ++i) {
			sf::Vector2f btnPos = {
				m_dropdownChoicesArea.getPosition().x + PADDING,
				m_dropdownChoicesArea.getPosition().y + outerPadding + (i - m_scrollOffset) * choiceHeight + (PADDING / 2.f)
			};

			m_optionButtons[i].setPosition(btnPos);
		}
		m_scrollBar.setPosition({
			m_dropdownChoicesArea.getPosition().x + m_dropdownChoicesArea.getSize().x - m_scrollBar.getSize().x - outerPadding,
			m_dropdownChoicesArea.getPosition().y + outerPadding + (static_cast<float>(m_scrollOffset) / static_cast<float>(m_options.size())) * (m_dropdownChoicesArea.getSize().y - (outerPadding * 2))
		});
	}
};

class SettingsScene : public Scene {
	inline static float SETTINGS_SCALE_FACTOR = Settings::VIRTUAL_WIDTH / 1280.0F;

	inline static constexpr sf::Vector2f CATEGORY_BUTTON_SIZE = { 48.0F,48.0F };
	inline static constexpr sf::Vector2i CATEGORY_BUTTON_TEXTURE_SIZE = { 15,15 };
	
	inline static float CATEGORY_BUTTON_ADJUSTED_PADDING = 8.0F * SETTINGS_SCALE_FACTOR;
	inline static float BACKGROUND_ADJUSTED_PADDING = 48.0F * SETTINGS_SCALE_FACTOR;
	inline static sf::Vector2f CATEGORY_BUTTON_ADJUSTED_SIZE = { CATEGORY_BUTTON_SIZE.x * SETTINGS_SCALE_FACTOR, CATEGORY_BUTTON_SIZE.y * SETTINGS_SCALE_FACTOR };

	inline static constexpr sf::Color IDLE_BUTTON_COLOR{ 255,255,255,255 };
	inline static constexpr sf::Color HOVER_BUTTON_COLOR{ 200,200,200,255 };
	inline static constexpr sf::Color ACTIVE_BUTTON_COLOR{ 150,150,150,255 };

	inline static float SETTINGS_ELEMENT_HEIGHT = 55.0F * SETTINGS_SCALE_FACTOR;

	enum class SettingsCategoryId : size_t {
		Graphics,
		Audio,
		Misc
	};
	struct CategoryButton {
		SettingsCategoryId id;
		ToggleButton button;
	};
	struct CategoryElement {
		SettingsCategoryId category;
		std::unique_ptr<SettingsElement> element;	
	};

	std::vector<CategoryButton> m_categories{};
	SettingsCategoryId m_currentCategory = SettingsCategoryId::Graphics;

	sf::RectangleShape m_categoriesBackground{ {CATEGORY_BUTTON_ADJUSTED_SIZE.x + (CATEGORY_BUTTON_ADJUSTED_PADDING * 2), Settings::VIRTUAL_HEIGHT} };
	sf::RectangleShape m_background{ {Settings::VIRTUAL_WIDTH, Settings::VIRTUAL_HEIGHT} };
	sf::RectangleShape m_settingsBackground{ {Settings::VIRTUAL_WIDTH - (m_categoriesBackground.getSize().x + (BACKGROUND_ADJUSTED_PADDING * 2)), Settings::VIRTUAL_HEIGHT - (BACKGROUND_ADJUSTED_PADDING * 2)} };

	sf::Text m_activeCategoryText{ Registry::getFont(), "Settings", Settings::SCALED_FONT_SIZE * 2 };

	Button m_backButton;
	bool m_returningToMenu = false;

	std::vector<CategoryElement> m_elements{};

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
		switch (id) {
		case SettingsCategoryId::Graphics:
			m_activeCategoryText.setString("Graphics");
			break;
		case SettingsCategoryId::Audio:
			m_activeCategoryText.setString("Audio");
			break;
		case SettingsCategoryId::Misc:
			m_activeCategoryText.setString("Miscellaneous");
			break;
		default:
			break;
		}
		for(auto& cat : m_categories) {
			cat.button.setToggled(cat.id == id);
		}
	}

	std::string getResolutionString(const sf::Vector2u& res) {
		return std::to_string(res.x) + "x" + std::to_string(res.y);
	}

	sf::Vector2f calculateSettingPosition(const sf::Vector2f& size, const sf::Vector2f& startPos, const int index = 0) {
		return {
			startPos.x,
			startPos.y + (static_cast<float>(index) * (size.y + CATEGORY_BUTTON_ADJUSTED_PADDING))
		};
	}

	void forceCloseAllDropdowns() {
		for (const auto& setting : m_elements) {
			if (setting.element->type() != SettingsElement::ElementType::Dropdown) continue;
			DropdownSetting& dropdown = static_cast<DropdownSetting&>(*setting.element);
			if (dropdown.isExpanded()) dropdown.forceExpanded(false);
		}
	}

	void createGraphicsOptions(const sf::Vector2f& size, const sf::Vector2f& startPos) {
		#pragma region Resolution dropdown

		// resolution dropdown
		std::vector<std::string> resolutions{ "768x768","1920x1080","1920x1200","512x512" };
		
		std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
		for (const auto& mode : modes) {
			if (mode.size.x < 800 || mode.size.y < 600) continue; // skip some :3
			const auto rstr = getResolutionString(mode.size);
			if (std::find(resolutions.begin(), resolutions.end(), rstr) == resolutions.end()) {
				resolutions.push_back(rstr);
			}
			std::cout << "vm " << mode.size.x << "x" << mode.size.y << std::endl;
		}

		const auto resStr = getResolutionString({ static_cast<unsigned int>(Settings::VIRTUAL_WIDTH),static_cast<unsigned int>(Settings::VIRTUAL_HEIGHT) });
		if (std::find(resolutions.begin(), resolutions.end(), resStr) == resolutions.end()) {
			resolutions.push_back(resStr);
		}

		std::sort(resolutions.begin(), resolutions.end(), [](const std::string& a, const std::string& b) {
			size_t axpos = a.find('x');
			size_t bxpos = b.find('x');
			if (axpos == std::string::npos || bxpos == std::string::npos) return a < b;
			unsigned int awidth = static_cast<unsigned int>(std::stoi(a.substr(0, axpos)));
			unsigned int aheight = static_cast<unsigned int>(std::stoi(a.substr(axpos + 1)));
			unsigned int bwidth = static_cast<unsigned int>(std::stoi(b.substr(0, bxpos)));
			unsigned int bheight = static_cast<unsigned int>(std::stoi(b.substr(bxpos + 1)));
			if (awidth == bwidth) {
				return aheight > bheight;
			}
			return awidth > bwidth;
		});

		
		size_t currentResIndex = 0;
		for (size_t i = 0; i < resolutions.size(); ++i) {
			if (resolutions[i] == resStr) {
				std::cout << "res id: " << currentResIndex << std::endl;
				currentResIndex = i;
				break;
			}
		}
		
		std::unique_ptr<DropdownSetting> resolutionDropdown = std::make_unique<DropdownSetting>(
			"resolution",
			"Resolution",
			size,
			startPos,
			resolutions,
			currentResIndex
		);
		resolutionDropdown->setChangeCallback([](SettingsElement& elem) {
			DropdownSetting& dropdown = static_cast<DropdownSetting&>(elem);
			const std::string& selected = dropdown.getSelectedOptionString();

			size_t xpos = selected.find('x');
			if (xpos == std::string::npos) return;

			std::string widthStr = selected.substr(0, xpos);
			std::string heightStr = selected.substr(xpos + 1);

			unsigned int width = static_cast<unsigned int>(std::stoi(widthStr));
			unsigned int height = static_cast<unsigned int>(std::stoi(heightStr));

			Config::changeResolution({ width, height });
			Scenery::load(SceneId::Settings, true);
		});
		m_elements.emplace_back(
			SettingsCategoryId::Graphics,
			std::move(resolutionDropdown)	
		);

		#pragma endregion
	}
	std::vector<std::string> createRangedDropdown(const int a, const int b, const int step = 5) {
		int total = std::abs(a - b);
		int count = total / step;

		if (total % step != 0) {
			std::cerr << "ranged dropdown generator: warning: total % step != 0" << std::endl;
		}

		std::vector<std::string> options{};
		options.reserve(count);

		for (int i = 0; i < (count + 1); i++) {
			options.push_back(std::to_string(a + (i * step)));
		}

		return options;
	}
	void createAudioOptions(const sf::Vector2f& size, const sf::Vector2f& startPos) {
		#pragma region Volume dropdowns

		std::unique_ptr<DropdownSetting> masterDropdown = std::make_unique<DropdownSetting>(
			"audiomaster",
			"Master volume",
			size,
			calculateSettingPosition(size, startPos, 0),
			createRangedDropdown(0, 100),
			20
		);
		masterDropdown->setChangeCallback([](SettingsElement& elem) {
			DropdownSetting& dropdown = static_cast<DropdownSetting&>(elem);
			Config::changeVolume(static_cast<float>(std::stoi(dropdown.getSelectedOptionString())));
		});

		std::unique_ptr<DropdownSetting> musicDropdown = std::make_unique<DropdownSetting>(
			"audiomusic",
			"Music volume",
			size,
			calculateSettingPosition(size, startPos, 1),
			createRangedDropdown(0, 100),
			20
		);
		musicDropdown->setChangeCallback([](SettingsElement& elem) {
			DropdownSetting& dropdown = static_cast<DropdownSetting&>(elem);
			Config::changeVolume(-1.0F,static_cast<float>(std::stoi(dropdown.getSelectedOptionString())));
		});

		std::unique_ptr<DropdownSetting> sfxDropdown = std::make_unique<DropdownSetting>(
			"audiosfx",
			"SFX volume",
			size,
			calculateSettingPosition(size, startPos, 2),
			createRangedDropdown(0, 100),
			20
		);
		sfxDropdown->setChangeCallback([](SettingsElement& elem) {
			DropdownSetting& dropdown = static_cast<DropdownSetting&>(elem);
			Config::changeVolume(-1.0F,-1.0F,static_cast<float>(std::stoi(dropdown.getSelectedOptionString())));
		});

		m_elements.emplace_back(SettingsCategoryId::Audio, std::move(masterDropdown));
		m_elements.emplace_back(SettingsCategoryId::Audio, std::move(musicDropdown));
		m_elements.emplace_back(SettingsCategoryId::Audio, std::move(sfxDropdown));

		#pragma endregion
	}
public:
	SettingsScene() : Scene(SceneId::Settings),
		m_backButton(
			CATEGORY_BUTTON_ADJUSTED_SIZE,
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
		const sf::Vector2f& mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		for(auto& cat : m_categories) {
			cat.button.update(mousePos);
			if (!cat.button.wasClicked()) continue;
			switchCategory(cat.id);
		}

		m_backButton.update(mousePos);
		if (m_backButton.wasClicked() && !m_returningToMenu) {
			Scenery::load(SceneId::Menu);
			m_returningToMenu = true;
		}

		for(auto& setting : m_elements) {
			if (setting.category != m_currentCategory) continue;
			setting.element->update(mousePos);
			if (setting.element->type() == SettingsElement::ElementType::Dropdown) {
				DropdownSetting& dropdown = static_cast<DropdownSetting&>(*setting.element);
				if (dropdown.justExpanded()) {
					forceCloseAllDropdowns();
					dropdown.forceExpanded(true);
				}
			}
		}
	}
	void draw(sf::RenderTarget& target, sf::RenderWindow& window, const TimeSnap& time) override {
		target.draw(m_background);
		target.draw(m_settingsBackground);
		target.draw(m_activeCategoryText);
		target.draw(m_categoriesBackground);	
		
		for(const auto& cat : m_categories) {
			cat.button.draw(target);
		}
		m_backButton.draw(target);

		for (auto& setting : m_elements) {
			if (setting.category != m_currentCategory) continue;
			setting.element->draw(target);
		}

		for (auto& setting : m_elements) {
			if (setting.category != m_currentCategory) continue;
			if (setting.element->type() != SettingsElement::ElementType::Dropdown) continue;
			DropdownSetting& dropdown = static_cast<DropdownSetting&>(*setting.element);
			dropdown.postDraw(target);
		}
	}

	void onLoad() override {
		addCategoryButton(SettingsCategoryId::Graphics);
		addCategoryButton(SettingsCategoryId::Audio);
		addCategoryButton(SettingsCategoryId::Misc);
		switchCategory(SettingsCategoryId::Graphics);

		m_categoriesBackground.setPosition({ 0.0F,0.0F });
		m_categoriesBackground.setFillColor({ 15,15,15,169 });

		m_background.setTexture(&Registry::getTexture("bgsettings"));

		m_settingsBackground.setFillColor({ 0,0,0,128 });
		m_settingsBackground.setPosition({ m_categoriesBackground.getSize().x + BACKGROUND_ADJUSTED_PADDING, BACKGROUND_ADJUSTED_PADDING });

		m_activeCategoryText.setPosition({ m_settingsBackground.getPosition().x + CATEGORY_BUTTON_ADJUSTED_PADDING, m_settingsBackground.getPosition().y + CATEGORY_BUTTON_ADJUSTED_PADDING - m_activeCategoryText.getCharacterSize() / 4});
		
		sf::FloatRect settingsArea = m_settingsBackground.getGlobalBounds();
		settingsArea.position += { CATEGORY_BUTTON_ADJUSTED_PADDING, CATEGORY_BUTTON_ADJUSTED_PADDING + m_activeCategoryText.getCharacterSize() + CATEGORY_BUTTON_ADJUSTED_PADDING };
		settingsArea.size -= { CATEGORY_BUTTON_ADJUSTED_PADDING * 2.0F, CATEGORY_BUTTON_ADJUSTED_PADDING * 2.0F + m_activeCategoryText.getCharacterSize() + CATEGORY_BUTTON_ADJUSTED_PADDING };

		const sf::Vector2f settingSize = { settingsArea.size.x - (CATEGORY_BUTTON_ADJUSTED_PADDING * 2.0F), SETTINGS_ELEMENT_HEIGHT };
		const sf::Vector2f settingPos = { settingsArea.position.x + CATEGORY_BUTTON_ADJUSTED_PADDING, settingsArea.position.y };

		createGraphicsOptions(settingSize, settingPos);
		createAudioOptions(settingSize, settingPos);
		Scene::onLoad();
	}
	void onUnload() override {
		Config::saveSettings();
		Scene::onUnload();
	}
	void onSFMLEvent(const std::optional<sf::Event>& ev) override {
		if (const auto& e = ev->getIf<sf::Event::MouseWheelScrolled>()) {
			for(auto& setting : m_elements) {
				if (setting.category != m_currentCategory) continue;
				if (setting.element->type() != SettingsElement::ElementType::Dropdown) continue;
				DropdownSetting& dropdown = static_cast<DropdownSetting&>(*setting.element);
				if (!dropdown.isExpanded()) continue;
				if (e->delta > 0) {
					dropdown.scroll(-1);
				}
				else if (e->delta < 0) {
					dropdown.scroll(1);
				}
			}
		}
	}
};

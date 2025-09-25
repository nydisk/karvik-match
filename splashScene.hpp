#pragma once
#include "sfx.hpp"
#include "scene.hpp"
#include "scenery.hpp"
#include "registry.hpp"
#include <SFML/Graphics.hpp>
class SplashScene : public Scene {
public:
	static constexpr float SPLASH_TIME = Transition::FADE_TIME + 3.0F;
private:
	bool m_splashIsOver = false;
	bool m_ding = false;
	float m_splashTimer = 0.0f;
	sf::RectangleShape m_splash{ {0.0F,0.0F} };
public:
	SplashScene() : Scene(SceneId::Splash) {}
	void update(sf::RenderTarget& target, sf::RenderWindow& window, const TimeSnap& time) override {
		m_splashTimer += time.delta;
		if (m_splashTimer >= Transition::FADE_TIME && !m_ding) {
			m_ding = true;
			SFX::play("splashfx");
		}
		if (m_splashTimer >= SPLASH_TIME && !m_splashIsOver) {
			m_splashIsOver = true;
			Scenery::load(SceneId::Menu);
		}
	}
	void draw(sf::RenderTarget& target, sf::RenderWindow& window, const TimeSnap& time) override {
		if (m_splash.getSize() == sf::Vector2f{0.0F, 0.0F}) m_splash.setSize(target.getView().getSize());
		target.draw(m_splash);
	}
	void onSFMLEvent(const std::optional<sf::Event>& ev) override {
		if (const auto& kpe = ev->getIf<sf::Event::KeyPressed>()) {
			if (kpe->code != sf::Keyboard::Key::Escape) return;
			m_splashTimer = SPLASH_TIME;
		}
	}
	void onUnload() override {
		Scene::onUnload();
	}
	void onLoad() override {
		m_splash.setTexture(&Registry::getTexture("splash"));
		Scene::onLoad();
	}
};
#pragma once
#include <SFML/Graphics.hpp>
#include "mathhelper.hpp"
#include "timesnap.hpp"
#include <functional>
enum class TransitionState {
	FadeIn,
	FadeOut,
	Nothing
};
class Transition {
public:
	inline static constexpr float FADE_TIME = 2.8f;
private:
	inline static float m_timer = 0.0f;
	inline static float m_alpha = 0.0f;
	inline static TransitionState m_state = TransitionState::Nothing;
	inline static sf::RectangleShape m_shape{};
	inline static std::function<void()> m_onComplete;
	inline static sf::Color getColor() {
		return {0,0,0,static_cast<uint8_t>(m_alpha * 255)};
	}
public:
	inline static void init(const sf::RenderWindow& window) {
		m_shape.setSize(window.getView().getSize());
		m_shape.setFillColor(sf::Color::Black);
	}
	inline static void post_draw(sf::RenderTarget& target) {
		if (m_state == TransitionState::Nothing) return;
		m_shape.setFillColor(getColor());
		target.draw(m_shape);
	}
	inline static void update(const TimeSnap& time) {
		if (m_state == TransitionState::Nothing) return;
		m_timer += time.delta;
		float t = math::smoothStep(std::clamp<float>(m_timer / FADE_TIME, 0.0F, 1.0F));
		switch (m_state) {
			case TransitionState::FadeIn:
				m_alpha = std::lerp(1.0F, 0.0F, t);
				if (math::approximately(m_alpha, 0.0F)) {
					m_state = TransitionState::Nothing;
					if (m_onComplete) { m_onComplete(); m_onComplete = nullptr; }
				}
				break;
			case TransitionState::FadeOut:
				m_alpha = std::lerp(0.0F, 1.0F, t);
				if (math::approximately(m_alpha, 1.0F)) {
					m_state = TransitionState::Nothing;
					if (m_onComplete) { m_onComplete(); m_onComplete = nullptr; }
				}
				break;
		}
	}
	inline static void fadeIn(std::function<void()> onComplete = nullptr) {
		m_state = TransitionState::FadeIn;
		m_timer = 0;
		m_alpha = 1.0F;
		m_onComplete = onComplete;
	}
	inline static void fadeOut(std::function<void()> onComplete = nullptr) {
		m_state = TransitionState::FadeOut;
		m_timer = 0;
		m_alpha = 0.0F;
		m_onComplete = onComplete;
	}
};
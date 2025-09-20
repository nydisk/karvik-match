#pragma once
#include <random>
class RNG {
	inline static std::random_device m_rd{};
	inline static std::mt19937 m_gen{m_rd()};
public:
	inline static std::mt19937& gen() {
		return m_gen;
	}
	// [0.0F, 1.0F]
	inline static float random() {
		return std::uniform_real_distribution<float>()(gen());
	}
	// [a, b]
	inline static float random(const float& a, const float& b) {
		return std::uniform_real_distribution<float>(a, b)(gen());
	}
	// [a, b]
	inline static int random(const int& a, const int& b) {
		return std::uniform_int_distribution<int>(a, b)(gen());
	}
	// [0, b]
	inline static int random(const int& b) {
		return std::uniform_int_distribution<int>(0, b)(gen());
	}
};
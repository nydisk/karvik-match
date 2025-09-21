#pragma once
namespace math {
	[[nodiscard]] inline static float smoothStep(const float t) {
		return t * t * (3.0f - 2.0f * t);
	}
	[[nodiscard]] inline static bool approximately(const float value, const float target, const float threshold = 0.005f) {
		return abs(target - value) <= threshold;
	}
}
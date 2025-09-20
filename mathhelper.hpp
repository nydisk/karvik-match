#pragma once
namespace math {
	[[nodiscard]] inline static float smoothStep(const float t) {
		return t * t * (3.0f - 2.0f * t);
	}
}
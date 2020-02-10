#include "Math.hpp"
#include <algorithm>
#include <functional>

namespace ofxPlanet {
namespace Math {
int floatToInt(float f) {
	return static_cast<int>(std::round(f));
}
float signum(float f) {
	if (isZero(f)) {
		return 0.0f;
	}
	if (f > 0) {
		return 1.0f;
	}
	return -1.0f;
}
float intbound(float s, float ds) {
	// Find the smallest positive t such that s+t*ds is an integer.
	if (ds < 0) {
		return intbound(-s, -ds);
	}
	else {
		s = std::fmod((std::fmod(s, 1.0f) + 1.0f), 1.0f);
		// problem is now s+t*ds = 1
		return (1.0f - s) / ds;
	}
}
bool isZero(float f, float limit) {
	float a = std::abs(f);
	return (a < limit);
}
}
}
#include "Math.hpp"
#include <algorithm>
namespace ofxPlanet {
namespace Math {
int floatToInt(float f) {
	return static_cast<int>(std::round(f));
}
}
}
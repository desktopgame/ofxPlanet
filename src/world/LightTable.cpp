#include "LightTable.hpp"

#include <stdexcept>
namespace ofxPlanet {
LightTable::LightTable(int xSize, int ySize, int zSize)
    : Array3D<int>(xSize, ySize, zSize, 0) {
}
float LightTable::computeShaderBrightness(int brightness) {
        float f = 1.0f - (static_cast<float>(brightness) /
                          static_cast<float>(LightTable::BRIGHTNESS_MAX));
        f = f * LightTable::BLACK;
        return f;
}
bool LightTable::testBrightness(int brightness) {
        return brightness >= BRIGHTNESS_MIN && brightness <= BRIGHTNESS_MAX;
}
void LightTable::verifyBrightness(int brightness) {
        if (!testBrightness(brightness)) {
                throw std::logic_error(
                    "light level is must be included in 0~15");
        }
}
}  // namespace ofxPlanet
#include "LightTable.hpp"

#include <stdexcept>
namespace ofxPlanet {
LightTable::LightTable(int xSize, int ySize, int zSize)
    : xSize(xSize), ySize(ySize), zSize(zSize), vec() {
        for (int x = 0; x < xSize; x++) {
                std::vector<std::vector<int> > yline;
                for (int y = 0; y < ySize; y++) {
                        std::vector<int> zline;
                        for (int z = 0; z < zSize; z++) {
                                zline.push_back(0);
                        }
                        yline.push_back(zline);
                }
                vec.push_back(yline);
        }
}
void LightTable::setLight(int x, int y, int z, int light) {
        verifyBrightness(light);
        vec[x][y][z] = light;
}
int LightTable::getLight(int x, int y, int z) const { return vec[x][y][z]; }
int LightTable::getXSize() const { return xSize; }
int LightTable::getYSize() const { return ySize; }
int LightTable::getZSize() const { return zSize; }
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
#pragma once
#ifndef WORLD_LIGHTTABLE_HPP
#define WORLD_LIGHTTABLE_HPP
#include <vector>
#include "Array3D.hpp"

namespace ofxPlanet {
/**
 * LightTable.
 */
class LightTable : public Array3D<int> {
       public:
        explicit LightTable(int xSize, int ySize, int zSize);

        /**
         * @param brightness
         * @return
         */
        static float computeShaderBrightness(int brightness);

        /**
         * @param brightness
         * @return
         */
        static bool testBrightness(int brightness);

        /**
         * @param brightness
         */
        static void verifyBrightness(int brightness);

        static constexpr int BRIGHTNESS_MIN = 0;
        static constexpr int BRIGHTNESS_MAX = 15;
        static constexpr float BLACK = 0.6f;

       private:
};
}  // namespace ofxPlanet
#endif
#pragma once
#ifndef WORLD_LIGHTTABLE_HPP
#define WORLD_LIGHTTABLE_HPP
#include <vector>

namespace ofxPlanet {
/**
 * LightTable.
 */
class LightTable {
       public:
        explicit LightTable(int xSize, int ySize, int zSize);

        /**
         * @param x
         * @param y
         * @param z
         * @param light
         */
        void setLight(int x, int y, int z, int light);
        /**
         * @param x
         * @param y
         * @param z
         * @return
         */
        int getLight(int x, int y, int z) const;

        /**
         * @return
         */
        int getXSize() const;
        /**
         * @return
         */
        int getYSize() const;
        /**
         * @return
         */
        int getZSize() const;

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
        int xSize, ySize, zSize;
        std::vector<std::vector<std::vector<int> > > vec;
};
}  // namespace ofxPlanet
#endif
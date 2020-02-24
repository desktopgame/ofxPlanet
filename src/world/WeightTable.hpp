#pragma once
#ifndef WORLD_WEIGHTTABLE_HPP
#define WORLD_WEIGHTTABLE_HPP
#include <set>
#include <vector>

#include "IntVec3Compare.hpp"

namespace ofxPlanet {
/**
 * WeightTable is for storaging weighting.
 */
class WeightTable {
       public:
        explicit WeightTable(int xSize, int ySize, int zSize);
        /**
         * add weight a specific position as center.
         * @param x
         * @param y
         * @param z
         * @param weight
         */
        void addWeight(int x, int y, int z, int weight);
        /**
         * overwrite weight for specific position.
         * @param x
         * @param y
         * @param z
         * @param weight
         */
        void setWeight(int x, int y, int z, int weight);

        /**
         * returns a weight for specific position.
         * @param x
         * @param y
         * @param z
         * @return
         */
        int getWeight(int x, int y, int z);

        /**
         * return X-axis from weight table 3D size.
         * @return
         */
        int getXSize() const;

        /**
         * return Y-axis from weight table 3D size.
         * @return
         */
        int getYSize() const;

        /**
         * return Z-axis from weight table 3D size.
         */
        int getZSize() const;

       private:
        void addWeight(int x, int y, int z, int weight,
                       std::set<glm::ivec3, IntVec3Compare>& set);
        int xSize, ySize, zSize;
        std::vector<std::vector<std::vector<int> > > vec;
};
}  // namespace ofxPlanet
#endif
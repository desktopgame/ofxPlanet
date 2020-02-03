#pragma once
#ifndef WORLD_WEIGHTTABLE_HPP
#define WORLD_WEIGHTTABLE_HPP
#include <set>
#include <vector>

#include "IntVec3Compare.hpp"

namespace planet {
class WeightTable {
       public:
        explicit WeightTable(int xSize, int ySize, int zSize);

        void addWeight(int x, int y, int z, int weight);

        void setWeight(int x, int y, int z, int weight);
        int getWeight(int x, int y, int z);

        int getXSize() const;
        int getYSize() const;
        int getZSize() const;

       private:
        void addWeight(int x, int y, int z, int weight,
                       std::set<glm::ivec3, KeyCompare>& set);
        int xSize, ySize, zSize;
        std::vector<std::vector<std::vector<int> > > vec;
};
}  // namespace planet
#endif
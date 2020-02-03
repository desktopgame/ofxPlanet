#pragma once
#ifndef WORLD_ENGINE_TERRAIN_HPP
#define WORLD_ENGINE_TERRAIN_HPP
#include <glm/glm.hpp>
#include <vector>

#include "Cell.hpp"
#include "Pocket.hpp"

namespace planet {
class Terrain {
       public:
        explicit Terrain();

        void addCell(const Cell& cell);
        Cell getCellAt(int index) const;
        int getCellCount() const;
        Cell findCell(int x, int z) const;

        void addPocket(const Pocket& pocket);
        Pocket getPocketAt(int index) const;
        int getPocketCount() const;

        glm::ivec3 getMin() const;
        glm::ivec3 getMax() const;

        std::vector<unsigned char> toPixelVec() const;

       private:
        glm::ivec3 vmin, vmax;
        std::vector<Cell> cells;
        std::vector<Pocket> pockets;
};
}  // namespace planet
#endif
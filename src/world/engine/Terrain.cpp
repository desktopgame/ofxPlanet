#include "Terrain.hpp"

#include <algorithm>
#include <iostream>

namespace planet {
Terrain::Terrain()
    : cells(),
      pockets(),
      vmin(32768, 32768, 32768),
      vmax(-32768, -32768, -32768) {}
void Terrain::addCell(const Cell& cell) {
        cells.emplace_back(cell);
        if (vmin.x > cell.x) {
                vmin.x = cell.x;
        }
        if (vmin.z > cell.z) {
                vmin.z = cell.z;
        }
        if (vmax.x < cell.x) {
                vmax.x = cell.x;
        }
        if (vmax.z < cell.z) {
                vmax.z = cell.z;
        }
}
Cell Terrain::getCellAt(int index) const { return cells.at(index); }
int Terrain::getCellCount() const { return static_cast<int>(cells.size()); }
Cell Terrain::findCell(int x, int z) const {
        auto iter = std::find_if(
            cells.begin(), cells.end(),
            [x, z](const Cell& e) -> bool { return e.x == x && e.z == z; });
        return *iter;
}
void Terrain::addPocket(const Pocket& pocket) { pockets.emplace_back(pocket); }
Pocket Terrain::getPocketAt(int index) const { return pockets.at(index); }
int Terrain::getPocketCount() const { return static_cast<int>(pockets.size()); }
glm::ivec3 Terrain::getMin() const { return vmin; }
glm::ivec3 Terrain::getMax() const { return vmax; }
std::vector<unsigned char> Terrain::toPixelVec() const {
        std::vector<unsigned char> v;
        for (int x = vmin.x; x < vmax.x; x++) {
                for (int z = vmin.z; z < vmax.z; z++) {
                        Cell c = findCell(x, z);
                        float noise = (c.noise + 1.f) / 2.0f;
                        float component = 255.f * noise;
                        char bit = static_cast<char>(component);
                        v.emplace_back(bit);
                        v.emplace_back(bit);
                        v.emplace_back(bit);
                        v.emplace_back(255);
                }
        }
        return v;
}
}  // namespace planet
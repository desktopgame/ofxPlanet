#include "BlockTable.hpp"

#include <algorithm>
#include <cassert>

#include "BlockPack.hpp"
#include "MultiBlock.hpp"
namespace ofxPlanet {
// BlockTable
BlockTable::BlockTable(int xSize, int ySize, int zSize)
    : Array3D<BlockPrefab>(xSize, ySize, zSize, BlockPrefab()), terrain() {
}

BlockTable::BlockTable() : BlockTable(-1,-1,-1) {}

std::vector<std::tuple<glm::ivec3, int>> BlockTable::expandTargets(
    int baseX, int baseY, int baseZ, const MultiBlock& mb) const {
        std::vector<std::tuple<glm::ivec3, int>> points;
        auto data = toCellVec(mb);
        for (auto e : data) {
                int x = baseX + e.point.x;
                int y = baseY + e.point.y;
                int z = baseZ + e.point.z;
                int id = e.blockId;
                points.emplace_back(
                    std::tuple<glm::ivec3, int>(glm::ivec3(x, y, z), id));
        }
        return points;
}

void BlockTable::expand(int baseX, int baseY, int baseZ, const MultiBlock& mb) {
        auto points = expandTargets(baseX, baseY, baseZ, mb);
        for (auto& point : points) {
                glm::ivec3 pos = std::get<0>(point);
                if (!contains(pos.x, pos.y, pos.z)) {
                        throw std::logic_error(
                            "position is out of bounds from table");
                }
                int id = std::get<1>(point);
                if (id < 0) {
                        set(pos.x, pos.y, pos.z, BlockPrefab(-1, false));
                } else {
                        set(pos.x, pos.y, pos.z, BlockPrefab(id, false));
                }
        }
}

bool BlockTable::canExpand(int baseX, int baseY, int baseZ,
                           const MultiBlock& mb) const {
        auto points = expandTargets(baseX, baseY, baseZ, mb);
        for (auto& point : points) {
                glm::ivec3 pos = std::get<0>(point);
                if (!contains(pos.x, pos.y, pos.z)) {
                        return false;
                }
                int oldId = get(pos.x, pos.y, pos.z).id;
                int newId = std::get<1>(point);
                if (newId < 0) {
                        continue;
                }
                if (oldId >= 0) {
                        return false;
                }
                if (pos.y > 0) {
                        glm::ivec3 bottom(pos.x, pos.y - 1, pos.z);
                        auto iter = std::find_if(
                            points.begin(), points.end(),
                            [bottom](auto e) -> bool {
                                    return std::get<0>(e) == bottom;
                            });
                        if (iter == points.end()) {
                                if (get(bottom.x, bottom.y, bottom.z).id ==
                                    -1) {
                                        return false;
                                }
                        }
                }
        }
        return true;
}

int BlockTable::getTopYForXZ(int x, int z) const {
        for (int i = ySize - 1; i >= 0; i--) {
                if (get(x, i, z).id != -1) {
                        return i;
                }
        }
        return ySize - 1;
}

std::vector<BlockArea> BlockTable::getAllBlockAreaForTop() const {
        std::vector<BlockArea> ret;
        std::set<glm::ivec3, IntVec3Compare> set;
        for (int x = 0; x < xSize; x++) {
                for (int z = 0; z < zSize; z++) {
                        int topY = getTopYForXZ(x, z);
                        glm::ivec3 pos(x, topY, z);
                        if (set.count(pos)) {
                                continue;
                        }
                        BlockArea area;
                        getAllBlockAreaForTopImpl(pos, set, area);
                        ret.emplace_back(area);
                }
        }
        return ret;
}

int BlockTable::getStackableHeight(const BlockArea& blockArea) const {
        auto points = blockArea.getPoints();
        int stack = 0;
        bool exit = false;
        while (!exit) {
                stack++;
                auto iter = points.begin();
                while (iter != points.end()) {
                        auto pos = *iter;
                        pos.y += stack;
                        if (pos.y >= getYSize() ||
                            get(pos.x, pos.y, pos.z).id != -1) {
                                exit = true;
                                break;
                        }
                        ++iter;
                }
        }
        return stack;
}

void BlockTable::setTerrain(const Terrain terrain) { this->terrain = terrain; }
Terrain BlockTable::getTerrain() const { return terrain; }
// private
void BlockTable::getAllBlockAreaForTopImpl(
    glm::ivec3 pos, std::set<glm::ivec3, IntVec3Compare>& set,
    BlockArea& area) const {
        set.insert(pos);
        area.addPoint(pos);

        glm::ivec3 xp = pos + glm::ivec3(1, 0, 0);
        glm::ivec3 xn = pos - glm::ivec3(1, 0, 0);
        glm::ivec3 yp = pos + glm::ivec3(0, 1, 0);
        glm::ivec3 yn = pos - glm::ivec3(0, 1, 0);
        glm::ivec3 zp = pos + glm::ivec3(0, 0, 1);
        glm::ivec3 zn = pos - glm::ivec3(0, 0, 1);
        addPos(pos, xp, set, area);
        addPos(pos, xn, set, area);
        // addPos(pos, yp, set, area);
        // addPos(pos, yn, set, area);
        addPos(pos, zp, set, area);
        addPos(pos, zn, set, area);
}
void BlockTable::addPos(glm::ivec3 pos, glm::ivec3 newPos,
                        std::set<glm::ivec3, IntVec3Compare>& set,
                        BlockArea& area) const {
        if (!set.count(newPos) && contains(newPos.x, newPos.y, newPos.z)) {
                set.insert(newPos);
                int y = getTopYForXZ(newPos.x, newPos.z);
                if (y == pos.y) {
                        getAllBlockAreaForTopImpl(newPos, set, area);
                }
        }
}
}  // namespace ofxPlanet

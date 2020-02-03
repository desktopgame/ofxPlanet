#include "BlockTable.hpp"

#include <algorithm>
#include <cassert>

#include "BlockPack.hpp"
#include "MultiBlock.hpp"
namespace planet {
// BlockPrefab
BlockPrefab::BlockPrefab(int id, bool instanced)
    : id(id), instanced(instanced) {}

BlockPrefab::BlockPrefab() : id(-1), instanced(false) {}

// BlockArea
BlockArea::BlockArea() : points() {}
void BlockArea::addPoint(glm::ivec3 point) { points.emplace_back(point); }
glm::ivec3 BlockArea::getPoint(int i) const { return points.at(i); }
int BlockArea::getPointCount() const { return static_cast<int>(points.size()); }
std::vector<glm::ivec3> BlockArea::getPoints() const { return points; }
glm::ivec3 BlockArea::compute2DSize() const {
        glm::ivec3 max(-32768, 0, -32768);
        glm::ivec3 min(32768, 0, 32768);
        for (auto& point : points) {
                min.x = std::min(min.x, point.x);
                min.z = std::min(min.z, point.z);
                max.x = std::max(max.x, point.x);
                max.z = std::max(max.z, point.z);
        }
        return max - min;
}
// BlockTable
BlockTable::BlockTable(int xSize, int ySize, int zSize)
    : xSize(xSize), ySize(ySize), zSize(zSize), terrain() {
        for (int x = 0; x < xSize; x++) {
                std::vector<std::vector<BlockPrefab>> xline;
                for (int y = 0; y < ySize; y++) {
                        std::vector<BlockPrefab> yline;
                        for (int z = 0; z < zSize; z++) {
                                yline.emplace_back(BlockPrefab());
                        }
                        xline.emplace_back(yline);
                }
                vec.emplace_back(xline);
        }
}

BlockTable::BlockTable() : xSize(-1), ySize(-1), zSize(-1), terrain() {}

void BlockTable::set(int x, int y, int z, const BlockPrefab& block) {
        vec[x][y][z] = block;
}

BlockPrefab& BlockTable::get(int x, int y, int z) { return vec[x][y][z]; }

const BlockPrefab& BlockTable::get(int x, int y, int z) const {
        return vec.at(x).at(y).at(z);
}

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
				//その座標はワールドからはみ出す
                if (!contains(pos.x, pos.y, pos.z)) {
                        return false;
                }
                int oldId = get(pos.x, pos.y, pos.z).id;
				//新しく設置されるブロックが空気なら無視
                int newId = std::get<1>(point);
                if (newId < 0) {
                        continue;
                }
                //既にブロックがある
                if (oldId >= 0) {
                        return false;
                }
				// 下にブロックがあるかどうかチェックする
				if (pos.y > 0) {
					// そもそもまだ下に構造物があるか確認
					glm::ivec3 bottom(pos.x, pos.y - 1, pos.z);
					auto iter = std::find_if(points.begin(), points.end(), [bottom](auto e) -> bool {
						return std::get<0>(e) == bottom;
					});
					// ない
					if (iter == points.end()) {
						// かつ、既に設置しているブロックが存在しない
						if (get(bottom.x, bottom.y, bottom.z).id == -1) {
							return false;
						}
					}
				}
        }
        return true;
}

bool BlockTable::contains(int x, int y, int z) const {
        if (x < 0 || y < 0 || z < 0) return false;
        if (x >= xSize || y >= ySize || z >= zSize) return false;
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
        std::set<glm::ivec3, KeyCompare> set;
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

int BlockTable::getXSize() const { return xSize; }

int BlockTable::getYSize() const { return ySize; }

int BlockTable::getZSize() const { return zSize; }
void BlockTable::setTerrain(const Terrain terrain) { this->terrain = terrain; }
Terrain BlockTable::getTerrain() const { return terrain; }
// private
void BlockTable::getAllBlockAreaForTopImpl(
    glm::ivec3 pos, std::set<glm::ivec3, KeyCompare>& set,
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
                        std::set<glm::ivec3, KeyCompare>& set,
                        BlockArea& area) const {
        if (!set.count(newPos) && contains(newPos.x, newPos.y, newPos.z)) {
                int y = getTopYForXZ(newPos.x, newPos.z);
                if (y == pos.y) {
                        getAllBlockAreaForTopImpl(newPos, set, area);
                }
        }
}
}  // namespace planet

#include "WeightTable.hpp"
namespace planet {
WeightTable::WeightTable(int xSize, int ySize, int zSize)
    : xSize(xSize), ySize(ySize), zSize(zSize), vec() {
        for (int i = 0; i < xSize; i++) {
                std::vector<std::vector<int> > gridYZ;
                for (int j = 0; j < ySize; j++) {
                        std::vector<int> lineZ;
                        for (int k = 0; k < zSize; k++) {
                                lineZ.emplace_back(0);
                        }
                        gridYZ.emplace_back(lineZ);
                }
                vec.emplace_back(gridYZ);
        }
}

void WeightTable::addWeight(int x, int y, int z, int weight) {
        std::set<glm::ivec3, KeyCompare> set;
        addWeight(x, y, z, weight, set);
}

void WeightTable::setWeight(int x, int y, int z, int weight) {
        vec[x][y][z] = weight;
}
int WeightTable::getWeight(int x, int y, int z) { return vec[x][y][z]; }
int WeightTable::getXSize() const { return xSize; }
int WeightTable::getYSize() const { return ySize; }
int WeightTable::getZSize() const { return zSize; }
// private
void WeightTable::addWeight(int x, int y, int z, int weight,
                            std::set<glm::ivec3, KeyCompare>& set) {
        glm::ivec3 pos(x, y, z);
        if (set.count(pos)) {
                return;
        }
        if (weight == 0) {
                return;
        }
        if (x < 0 || y < 0 || z < 0) return;
        if (x >= xSize || y >= ySize || z >= zSize) return;

        set.insert(pos);
        this->vec[x][y][z] = this->vec[x][y][z] + weight;

        glm::ivec3 xp = pos + glm::ivec3(1, 0, 0);
        glm::ivec3 xn = pos - glm::ivec3(1, 0, 0);
        glm::ivec3 yp = pos + glm::ivec3(0, 1, 0);
        glm::ivec3 yn = pos - glm::ivec3(0, 1, 0);
        glm::ivec3 zp = pos + glm::ivec3(0, 0, 1);
        glm::ivec3 zn = pos - glm::ivec3(0, 0, 1);

        addWeight(xp.x, xp.y, xp.z, weight - 1, set);
        addWeight(xn.x, xn.y, xn.z, weight - 1, set);
        addWeight(yp.x, yp.y, yp.z, weight - 1, set);
        addWeight(yn.x, yn.y, yn.z, weight - 1, set);
        addWeight(zp.x, zp.y, zp.z, weight - 1, set);
        addWeight(zn.x, zn.y, zn.z, weight - 1, set);
}
}  // namespace planet
#include "MultiBlock.hpp"

#include <algorithm>
#include <cassert>
#include <sstream>
#include <stdexcept>

#include "Block.hpp"
#include "BlockPack.hpp"

namespace planet {
MultiBlockCell::MultiBlockCell(const glm::ivec3& point, int blockId)
    : point(point), blockId(blockId) {}
void multiBlock2DSize(const MultiBlockLayer& layer, glm::ivec3& destSize) {
        int xSize = -1;
        for (auto& line : layer) {
                int lineSize = static_cast<int>(line.size());
                if (xSize == -1) {
                        xSize = lineSize;
                } else if (xSize < lineSize) {
                        xSize = lineSize;
                }
        }
        int zSize = static_cast<int>(layer.size());
        destSize.x = xSize;
        destSize.z = zSize;
}

void multiBlock3DSize(const MultiBlock& multiBlock, glm::ivec3& destSize) {
        int ySize = static_cast<int>(multiBlock.size());
        glm::ivec3 size;
        bool first = true;
        for (auto& layer : multiBlock) {
                glm::ivec3 temp;
                multiBlock2DSize(layer, temp);
                if (first) {
                        size = temp;
                } else if (size != temp) {
                        size.x = std::max(size.x, temp.x);
                        size.z = std::max(size.z, temp.z);
                }
        }
        size.y = ySize;
        destSize = size;
}
std::vector<MultiBlockCell> toCellVec(const MultiBlock& block) {
        std::vector<MultiBlockCell> ret;
        auto blockPack = BlockPack::getCurrent();
        for (int y = 0; y < static_cast<int>(block.size()); y++) {
                auto& layer = block.at(y);
                for (int z = 0; z < static_cast<int>(layer.size()); z++) {
                        auto& line = layer.at(z);
                        for (int x = 0; x < static_cast<int>(line.size());
                             x++) {
                                auto& blockName = line.at(x);
                                int blockIndex =
                                    blockPack->getBlockIndexForName(blockName);
                                // TODO:ÉuÉçÉbÉNÇ™Ç»Ç©Ç¡ÇΩèÍçá
                                MultiBlockCell cell(glm::ivec3(x, y, z),
                                                    blockIndex);
                                ret.emplace_back(cell);
                        }
                }
        }
        return ret;
}
}  // namespace planet
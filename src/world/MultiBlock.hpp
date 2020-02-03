#pragma once
#ifndef GAME_WORLD_MULTIBLOCK_HPP
#define GAME_WORLD_MULTIBLOCK_HPP
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../common/GLM.hpp"
namespace planet {

struct MultiBlockCell {
        explicit MultiBlockCell(const glm::ivec3& point, int blockId);
        glm::ivec3 point;
        int blockId;
};

using MultiBlockLine = std::vector<std::string>;
using MultiBlockLayer = std::vector<MultiBlockLine>;
using MultiBlock = std::vector<MultiBlockLayer>;

void multiBlock2DSize(const MultiBlockLayer& layer, glm::ivec3& destSize);
void multiBlock3DSize(const MultiBlock& multiBlock, glm::ivec3& destSize);
std::vector<MultiBlockCell> toCellVec(const MultiBlock& block);
}  // namespace planet
#endif
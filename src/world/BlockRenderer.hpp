#pragma once
#ifndef WORLD_BLOCKRENDERER_HPP
#define WORLD_BLOCKRENDERER_HPP
#include "CubeRenderer.hpp"

namespace planet {
class World;
class BlockRenderer {
       public:
        explicit BlockRenderer(const World& world, ofShader& shader);

        void clear();
        void update();
        void render();

        CubeRenderer& getCubeRenderer();
        CubeRenderer& getLeftSlabRenderer();
        CubeRenderer& getRightSlabRenderer();
        CubeRenderer& getFrontSlabRenderer();
        CubeRenderer& getBackSlabRenderer();
        CubeRenderer& getTopSlabRenderer();
        CubeRenderer& getBottomSlabRenderer();

       private:
        CubeRenderer cubeRenderer;
        CubeRenderer leftSlabRenderer;
        CubeRenderer rightSlabRenderer;
        CubeRenderer frontSlabRenderer;
        CubeRenderer backSlabRenderer;
        CubeRenderer topSlabRenderer;
        CubeRenderer bottomSlabRenderer;
};
}  // namespace planet
#endif
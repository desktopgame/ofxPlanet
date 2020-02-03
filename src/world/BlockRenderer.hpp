#pragma once
#ifndef WORLD_BLOCKRENDERER_HPP
#define WORLD_BLOCKRENDERER_HPP
#include "CubeRenderer.hpp"

namespace ofxPlanet {
class World;
/**
 * BlockRenderer is renderer for rendering block.
 */
class BlockRenderer {
       public:
        explicit BlockRenderer(const World& world, ofShader& shader);

        /**
         * clear all renderer.
         */
        void clear();
        /**
         * update all renderer.
         */
        void update();
        /**
         * draw all renderer.
         */
        void render();

        /**
         * returns renderer for render a cube.
         * @return
         */
        CubeRenderer& getCubeRenderer();
        /**
         * returns renderer for render a slab.
         * @return
         */
        CubeRenderer& getLeftSlabRenderer();
        /**
         * returns renderer for render a slab.
         * @return
         */
        CubeRenderer& getRightSlabRenderer();
        /**
         * returns renderer for render a slab.
         * @return
         */
        CubeRenderer& getFrontSlabRenderer();
        /**
         * returns renderer for render a slab.
         * @return
         */
        CubeRenderer& getBackSlabRenderer();
        /**
         * returns renderer for render a slab.
         * @return
         */
        CubeRenderer& getTopSlabRenderer();
        /**
         * returns renderer for render a slab.
         * @return
         */
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
}  // namespace ofxPlanet
#endif
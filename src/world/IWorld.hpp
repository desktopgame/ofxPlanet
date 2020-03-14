#pragma once
#ifndef WORLD_IWORLD_HPP
#define WORLD_IWORLD_HPP
#include <ofShader.h>

#include <glm/glm.hpp>
#include <memory>

#include "ChunkLoadStyle.hpp"
#include "IAreaBounds.hpp"
#include "LightTable.hpp"

namespace ofxPlanet {
class Block;
class Chunk;
class Sector;
class IWorld : public IAreaBounds {
       public:
        using Instance = std::shared_ptr<IWorld>;
        using Reference = std::weak_ptr<IWorld>;
        explicit IWorld() = default;
        virtual ~IWorld() {}

        /**
         * recompute light table if marked as `taint`.
         */
        virtual void computeBrightness() = 0;

        /**
         * returns a Y-axis from world 3D size.
         * @return
         */
        virtual int getYSize() const = 0;
        /**
         * returns a block for specific position.
         * @param x
         * @param y
         * @param z
         * @return
         */
        virtual std::shared_ptr<Block> getBlock(int x, int y, int z) const = 0;

        /**
         * @param x
         * @param y
         * @param z
         * @return
         */
        virtual int getBrightness(int x, int y, int z) const = 0;

        /**
         * returns shader.
         * @return
         */
        virtual ofShader& getShader() = 0;

        /**
         * @return
         */
        virtual glm::vec3 getViewPosition() const = 0;

        /**
         * @return
         */
        virtual int getViewRange() const = 0;

        /**
         * @return
         */
        virtual ChunkLoadStyle getChunkLoadStyle() const = 0;

        /**
         * @param xOffset
         * @param zOffset
         * @return
         */
        virtual std::shared_ptr<Sector> getSector(int xOffset,
                                                  int zOffset) const = 0;

        /**
         * @return
         */
        virtual std::shared_ptr<Chunk> getCurrentChunk() = 0;

       private:
};
}  // namespace ofxPlanet
#endif
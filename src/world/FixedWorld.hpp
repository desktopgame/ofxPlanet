#pragma once
#ifndef WORLD_FIXEDWORLD_HPP
#define WORLD_FIXEDWORLD_HPP
#include <ofFbo.h>

#include <array>
#include <glm/glm.hpp>
#include <memory>

#include "BlockRenderer.hpp"
#include "BlockTable.hpp"
#include "IWorld.hpp"
#include "LightTable.hpp"
namespace ofxPlanet {

class Camera;
class FixedWorld;
/**
 * WorldPart is part from world.
 */
class WorldPart {
       public:
        explicit WorldPart(const std::shared_ptr<FixedWorld>& world,
                           glm::ivec3 offset);

        std::shared_ptr<FixedWorld> world;
        glm::ivec3 offset;
};
/**
 * RaycastResult.
 */
struct RaycastResult {
        explicit RaycastResult();
        glm::vec3 normal;
        glm::ivec3 position;
        bool hit;
};

class Entity;
class Block;
class Chunk;
/**
 * FixedWorld is three dimenstional world composed cube.
 */
class FixedWorld : public IWorld {
       public:
        // Utility
        /**
         * returns a new world.
         * @param shader
         * @param size
         * @return
         */
        static std::shared_ptr<FixedWorld> create(ofShader& shader,
                                                  const glm::ivec3& size);

        /**
         * returns a new world.
         * @param shader
         * @param xSize
         * @param ySize
         * @param zSize
         * @return
         */
        static std::shared_ptr<FixedWorld> create(ofShader& shader, int xSize,
                                                  int ySize, int zSize);

        // IWorld
        void computeBrightness();
        int getYSize() const;
        std::shared_ptr<Block> getBlock(int x, int y, int z) const;
        bool isFilled(int x, int y, int z) const;
        int getBrightness(int x, int y, int z) const;
        ofShader& getShader();
        glm::vec3 getViewPosition() const;
        int getViewRange() const;
        ChunkLoadStyle getChunkLoadStyle() const;
        std::shared_ptr<Sector> getSector(int xOffset, int zOffset) const;
        std::shared_ptr<Chunk> getCurrentChunk();
		void setSunBrightness(int sunBrightness);
		int getSunBrightness() const;

        /**
         * overwrite blocks by specified table.
         * @param table
         */
        void load(const BlockTable& table);
        /**
         * remove all blocks.
         */
        void clear();

        /**
         * update all blocks.
         */
        void update();

        /**
         * mark as `taint` a light table.
         */
        void invalidateBrightness();

        /**
         * porting from stackexchange.
         * @param origin
         * @param direction
         * @param length
         * @param scale
         * @return
         * @see
         * https://gamedev.stackexchange.com/questions/47362/cast-ray-to-select-block-in-voxel-game
         */
        RaycastResult raycast(glm::vec3 origin, glm::vec3 direction,
                              float length, float scale = 2.0f) const;

        /**
         * overwrite block for specific position.
         * @param pos
         * @param block
         */
        void setBlock(glm::ivec3 pos, std::shared_ptr<Block> block);
        /**
         * overwrite block for specific position.
         * @param x
         * @param y
         * @param z
         * @return block
         */
        void setBlock(int x, int y, int z, std::shared_ptr<Block> block);

        /**
         * returns a block for specific position.
         * @param pos
         * @return
         */
        std::shared_ptr<Block> getBlock(glm::ivec3 pos) const;

        /**
         * @param x
         * @param z
         * @return
         */
        int getTopYForXZ(int x, int z) const;

        /**
         * return true if included in world a specific position.
         * @param x
         * @param y
         * @param z
         * @return
         */
        bool isContains(int x, int y, int z) const;
        /**
         * return true if included in world a specific position.
         * @param v
         * @return
         */
        bool isContains(const glm::ivec3& v) const;
        /**
         * return true if empty a specified position, or outside from world.
         * @param x
         * @param y
         * @param z
         * @return
         */
        bool isEmpty(int x, int y, int z) const;

        /**
         * return height visible from top-view for specific position.
         * @param x
         * @param z
         * @return
         */
        int getGroundY(int x, int z) const;

        /**
         * returns a physical position in OpenGL for specific block.
         * @param x
         * @param y
         * @param z
         */
        glm::vec3 getPhysicalPosition(int x, int y, int z) const;

        /**
         * returns a X-axis from world 3D size.
         * @return
         */
        int getXSize() const;

        /**
         * returns a Z-axis from world 3D size.
         * @return
         */
        int getZSize() const;

        /**
         * returns a world 3D size.
         * @return
         */
        glm::ivec3 getSize() const;

        /**
         * split a world for specific split count.
         * @param splitNum
         * @return
         */
        std::vector<WorldPart> split(int splitNum) const;

        /**
         * returns chunk.
         * @return
         */
        std::shared_ptr<Chunk> getChunk() const;

        /**
         * @param chunkLoadStyle
         */
        void setChunkLoadStyle(ChunkLoadStyle chunkLoadStyle);

        /**
         * @param viewPosition
         */
        void setViewPosition(const glm::vec3& viewPosition);

        /**
         * @param viewRange
         */
        void setViewRange(int viewRange);

       private:
        explicit FixedWorld(ofShader& shader, const glm::ivec3& size);
        explicit FixedWorld(ofShader& shader, int xSize, int ySize, int zSize);
        void updateNeighborChunks();
		void lightDiffusion(int x, int y, int z, int brightness);
		void lightDiffusion2(int x, int y, int z, int brightness, std::vector<glm::ivec3>& v);

        std::vector<std::vector<std::vector<std::shared_ptr<Block> > > > blocks;
        int xSize, ySize, zSize;
        ofShader& shader;
        LightTable lightTable;
        std::shared_ptr<Chunk> chunk;
        ChunkLoadStyle chunkLoadStyle;
        glm::vec3 viewPosition;
        int viewRange;
        std::shared_ptr<Chunk> currentChunk;
		int sunBrightness;

        bool invalidBrightCache;
};
}  // namespace ofxPlanet
#endif
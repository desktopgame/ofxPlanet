#pragma once
#ifndef WORLD_CHUNK_HPP
#define WORLD_CHUNK_HPP
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "BlockRenderer.hpp"
#include "IWorld.hpp"

namespace ofxPlanet {
/**
 * ChunkType.
 */
enum class ChunkType {
        Single,
        Split,
};
/**
 * Chunk is part of world.
 */
class Chunk : public std::enable_shared_from_this<Chunk> {
       public:
        using Instance = std::shared_ptr<Chunk>;
        using Reference = std::weak_ptr<Chunk>;
        ~Chunk();
        /**
         * returns new chunk.
         * @param world
         * @param xOffset
         * @param yOffset
         * @param xSize
         * @param zSize
         * @return
         */
        static Instance create(IWorld& world, int xOffset, int zOffset,
                               int xSize, int zSize);
        /**
         * this chunk mark as `taint`.
         */
        void invalidate();
        /**
         * this chunk mark as `taint`.
         * @param x
         * @param y
         * @param z
         */
        void invalidate(int x, int y, int z);
        /**
         * recompute vertex information, if marked as `taint`.
         */
        void rehash();
        /**
         * draw chunk.
         */
        void draw();

        /**
         * draw chunk range.
         */
        void debugDraw();

        /**
         * return true if included in chunk for specific position.
         * @param x
         * @param y
         * @param z
         * @return
         */
        bool isContains(int x, int y, int z) const;

        /**
         * @param pos
         * @return
         */
        bool isContains(const glm::ivec3& pos) const;

        /**
         * return true if marked as `taint`.
         * @return
         */
        bool isInvalid() const;
        /**
         * returns world.
         * @return
         */
        const IWorld& getWorld() const;

        /**
         * split chunks.
         * @param splitSize
         */
        void split(int splitSize);

        /**
         * returns type.
         * @return
         */
        ChunkType getType() const;

        /**
         * returns parent.
         * @return
         */
        Instance getParent() const;

        /**
         * @param x
         * @param y
         * @param z
         * @return
         */
        Instance lookup(int x, int y, int z) const;

        /**
         * @param pos
         * @return
         */
        Instance lookup(const glm::ivec3& pos) const;

		/**
		 * @param chunk
		 * @param viewRange
		 * @return
		 */
		std::vector<Instance> getNeighbor(const Instance& chunk, int viewRange) const;

		/**
		 * @param index
		 * @return
		 */
		Instance getSubChunk(int index) const;

		/**
		 * @return
		 */
		int getSubChunkCount() const;

		/**
		 * @return
		 */
		int getXOffset() const;

		/**
		 * @return
		 */
		int getZOffset() const;

		/**
		 * @return
		 */
		int getXSize() const;

		/**
		 * @return
		 */
		int getZSize() const;

		/**
		 * @param enabled
		 */
		void setVisible(bool visible);

		/**
		 * @return
		 */
		bool isVisible() const;

		/**
		 * set visible this chunk, and all sub chunks.
		 */
		void show();

		/**
		 * set not visible this chunk, and all sub chunks.
		 */
		void hide();

		void tidyResource();

       private:
        explicit Chunk(Reference parent, IWorld& world, int xOffset, int zOffset,
                       int xSize, int zSize);
		static void setVisibleRecursive(Instance chunk, bool visible);
        void allocateRenderer();
        void deleteRenderer();
        void batch();
        void rehashAll();
        void rehashVisible();

        ChunkType type;
        bool invalid;
		bool visible;
        IWorld& world;
        BlockRenderer* renderer;
        int xOffset;
        int zOffset;
        int xSize;
        int zSize;
        Reference parent;
        std::vector<Instance> subchunks;
};
}  // namespace ofxPlanet
#endif
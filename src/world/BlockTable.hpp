#pragma once
#ifndef WORLD_BLOCKTABLE_HPP
#define WORLD_BLOCKTABLE_HPP
#include <set>
#include <tuple>
#include <vector>

#include "IntVec3Compare.hpp"
#include "MultiBlock.hpp"
#include "BlockPrefab.hpp"
#include "BlockArea.hpp"
#include "engine/Terrain.hpp"

namespace ofxPlanet {

/**
 * BlockTable is cotain BlockPrefab in three dimensional array.
 * using as intermediate representation for world generation process.
 */
class BlockTable {
       public:
        explicit BlockTable(int xSize, int ySize, int zSize);
        BlockTable();
        /**
         * overwrite BlockPrefab for specific position.
         * @param x
         * @param y
         * @param z
         * @param block
         */
        void setBlock(int x, int y, int z, const BlockPrefab& block);

        /**
         * returns BlockPrefab for specific position.
         * @param x
         * @param y
         * @param z
         * @return
         */
        BlockPrefab& getBlock(int x, int y, int z);

        /**
         * returns BlockPrefab for specific position.
         * @param x
         * @param y
         * @param z
         * @return
         */
        const BlockPrefab& getBlock(int x, int y, int z) const;

        /**
         * returns a generation points for specific structure at specific
         * position.
         * @param baseX
         * @param baseY
         * @param baseZ
         * @param mb
         * @return
         */
        std::vector<std::tuple<glm::ivec3, int> > expandTargets(
            int baseX, int baseY, int baseZ, const MultiBlock& mb) const;
        /**
         * generate structure at specific position.
         * @param baseX
         * @param baseY
         * @param baseZ
         * @param mb
         */
        void expand(int baseX, int baseY, int baseZ, const MultiBlock& mb);

        /**
         * return true if can generate structure at specific position.
         * @param baseX
         * @param baseY
         * @param baseZ
         * @param mb
         */
        bool canExpand(int baseX, int baseY, int baseZ,
                       const MultiBlock& mb) const;

        /**
         * return true if included in table a specific position.
         * @param x
         * @param y
         * @param z
         * @return
         */
        bool contains(int x, int y, int z) const;
        /**
         * returns most high Y point for specific position.
         * @param x
         * @param z
         * @return
         */
        int getTopYForXZ(int x, int z) const;
        /**
         * returns a list of area for top-view.
         * @return
         */
        std::vector<BlockArea> getAllBlockAreaForTop() const;

        /**
         * returns a stackable height for area.
         * @param blockArea
         * @return
         */
        int getStackableHeight(const BlockArea& blockArea) const;

        /**
         * returns x-axis from table size.
         * @return
         */
        int getXSize() const;
        /**
         * returns y-axis from table size.
         * @return
         */
        int getYSize() const;
        /**
         * returns z-axis from table size.
         * @return
         */
        int getZSize() const;

        /**
         * overwrite terrain.
         * @param terrain
         */
        void setTerrain(const Terrain terrain);
        /**
         * returns terrain.
         * @return
         */
        Terrain getTerrain() const;

       private:
        void getAllBlockAreaForTopImpl(
            glm::ivec3 pos, std::set<glm::ivec3, IntVec3Compare>& set,
            BlockArea& area) const;
        void addPos(glm::ivec3 pos, glm::ivec3 newPos,
                    std::set<glm::ivec3, IntVec3Compare>& set,
                    BlockArea& area) const;
        int xSize, ySize, zSize;
        Terrain terrain;
        std::vector<std::vector<std::vector<BlockPrefab> > > vec;
};
}  // namespace ofxPlanet
#endif
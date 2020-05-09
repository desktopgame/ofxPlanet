#pragma once
#ifndef WORLD_BLOCKTABLE_HPP
#define WORLD_BLOCKTABLE_HPP
#include <set>
#include <tuple>
#include <vector>

#include "Array3D.hpp"
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
class BlockTable : public Array3D<BlockPrefab> {
       public:
        explicit BlockTable(int xSize, int ySize, int zSize);
        BlockTable();

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
        Terrain terrain;
};
}  // namespace ofxPlanet
#endif
#pragma once
#ifndef WORLD_BLOCKTABLE_HPP
#define WORLD_BLOCKTABLE_HPP
#include <set>
#include <tuple>
#include <vector>

#include "IntVec3Compare.hpp"
#include "MultiBlock.hpp"
#include "engine/Terrain.hpp"

namespace planet {
struct BlockPrefab {
        explicit BlockPrefab(int id, bool instanced);
        explicit BlockPrefab();
        int id;
        bool instanced;
};

/**
 * BlockArea は、同じY座標に存在するブロック座標の一覧です。
 */
class BlockArea {
       public:
        explicit BlockArea();

        void addPoint(glm::ivec3 point);
        glm::ivec3 getPoint(int i) const;
        int getPointCount() const;
        std::vector<glm::ivec3> getPoints() const;

        glm::ivec3 compute2DSize() const;

       private:
        std::vector<glm::ivec3> points;
};

class BlockTable {
       public:
        explicit BlockTable(int xSize, int ySize, int zSize);
        BlockTable();
        void set(int x, int y, int z, const BlockPrefab& block);
        BlockPrefab& get(int x, int y, int z);
        const BlockPrefab& get(int x, int y, int z) const;

        std::vector<std::tuple<glm::ivec3, int> > expandTargets(
            int baseX, int baseY, int baseZ, const MultiBlock& mb) const;
        void expand(int baseX, int baseY, int baseZ, const MultiBlock& mb);
        bool canExpand(int baseX, int baseY, int baseZ,
                       const MultiBlock& mb) const;

        /**
         * 指定の座標が範囲ならtrue.
         * @param x
         * @param y
         * @param z
         * @return
         */
        bool contains(int x, int y, int z) const;
        /**
         * 指定のXZ座標内でもっとも高い位置のY座標を返します。
         * @param x
         * @param z
         * @return
         */
        int getTopYForXZ(int x, int z) const;
        /**
         * XZ方向全てを検査して、もっとも高いブロック座標を基準とした BlockArea
         * の一覧を返します。
         * @return
         */
        std::vector<BlockArea> getAllBlockAreaForTop() const;

        /**
         * 指定の領域を上方向にどれだけ積み重ねることができるかを返します。
         * @param blockArea
         * @return
         */
        int getStackableHeight(const BlockArea& blockArea) const;

        int getXSize() const;
        int getYSize() const;
        int getZSize() const;

        void setTerrain(const Terrain terrain);
        Terrain getTerrain() const;

       private:
        void getAllBlockAreaForTopImpl(glm::ivec3 pos,
                                       std::set<glm::ivec3, KeyCompare>& set,
                                       BlockArea& area) const;
        void addPos(glm::ivec3 pos, glm::ivec3 newPos,
                    std::set<glm::ivec3, KeyCompare>& set,
                    BlockArea& area) const;
        int xSize, ySize, zSize;
        Terrain terrain;
        std::vector<std::vector<std::vector<BlockPrefab> > > vec;
};
}  // namespace planet
#endif
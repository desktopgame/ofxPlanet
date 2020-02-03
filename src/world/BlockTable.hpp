#pragma once
#ifndef WORLD_BLOCKTABLE_HPP
#define WORLD_BLOCKTABLE_HPP
#include <set>
#include <tuple>
#include <vector>

#include "IntVec3Compare.hpp"
#include "MultiBlock.hpp"
#include "engine/Terrain.hpp"

namespace ofxPlanet {
struct BlockPrefab {
        explicit BlockPrefab(int id, bool instanced);
        explicit BlockPrefab();
        int id;
        bool instanced;
};

/**
 * BlockArea �́A����Y���W�ɑ��݂���u���b�N���W�̈ꗗ�ł��B
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
         * �w��̍��W���͈͂Ȃ�true.
         * @param x
         * @param y
         * @param z
         * @return
         */
        bool contains(int x, int y, int z) const;
        /**
         * �w���XZ���W���ł����Ƃ������ʒu��Y���W��Ԃ��܂��B
         * @param x
         * @param z
         * @return
         */
        int getTopYForXZ(int x, int z) const;
        /**
         * XZ�����S�Ă��������āA�����Ƃ������u���b�N���W����Ƃ��� BlockArea
         * �̈ꗗ��Ԃ��܂��B
         * @return
         */
        std::vector<BlockArea> getAllBlockAreaForTop() const;

        /**
         * �w��̗̈��������ɂǂꂾ���ςݏd�˂邱�Ƃ��ł��邩��Ԃ��܂��B
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
}  // namespace ofxPlanet
#endif
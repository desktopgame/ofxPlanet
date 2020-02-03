#pragma once
#ifndef WORLD_BLOCKINFOCOLLECTION_HPP
#define WORLD_BLOCKINFOCOLLECTION_HPP
#include <string>
#include <vector>

namespace ofxPlanet {
/**
 * BlockInfo is block information for one block.
 */
struct BlockInfo {
        explicit BlockInfo() = default;
        std::string textue;
        std::string reference;
        std::string shape;
};
/**
 * BlockInfoCollection is collection of BlockInfo.
 */
class BlockInfoCollection {
       public:
        explicit BlockInfoCollection();
        /**
         * serialize as json.
         * @return
         */
        std::string serialize() const;
        /**
         * deserialize from json.
         * @param json
         */
        void deserialize(const std::string& json);

        /**
         * add BlockInfo.
         * @param blockInfo
         */
        void addBlockInfo(const BlockInfo& blockInfo);

        /**
         * returns BlockInfo from index.
         * @param index
         * @return
         */
        BlockInfo getBlockInfo(int index) const;

        /**
         * return true if found BlockInfo from reference.
         * @param reference
         * @param outBlockInfo
         * @return
         */
        bool tryGetBlockInfo(const std::string& reference,
                             BlockInfo& outBlockInfo);
        /**
         * returns count of BlockInfo.
         * @return
         */
        int getBlockInfoCount() const;

       private:
        std::vector<BlockInfo> blockInfoVec;
};
}  // namespace ofxPlanet
#endif
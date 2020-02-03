#pragma once
#ifndef WORLD_BLOCKINFOCOLLECTION_HPP
#define WORLD_BLOCKINFOCOLLECTION_HPP
#include <string>
#include <vector>

namespace planet {
struct BlockInfo {
        explicit BlockInfo() = default;
        std::string textue;
        std::string reference;
        std::string shape;
};

class BlockInfoCollection {
       public:
        explicit BlockInfoCollection();
        std::string serialize() const;
        void deserialize(const std::string& json);

        void addBlockInfo(const BlockInfo& blockInfo);
        BlockInfo getBlockInfo(int index) const;
        bool tryGetBlockInfo(const std::string& reference,
                             BlockInfo& outBlockInfo);
        int getBlockInfoCount() const;

       private:
        std::vector<BlockInfo> blockInfoVec;
};
}  // namespace planet
#endif
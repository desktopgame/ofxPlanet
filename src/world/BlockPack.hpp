#pragma once
#ifndef WORLD_BLOCKPACK_HPP
#define WORLD_BLOCKPACK_HPP
#include <memory>
#include <vector>

#include "BlockInfoCollection.hpp"

namespace planet {
class Block;
class BlockPack {
       public:
        static std::shared_ptr<BlockPack> load(
            const BlockInfoCollection& blockInfoCollection);
        static std::shared_ptr<BlockPack> make(const std::string& name);
        static std::shared_ptr<BlockPack> getCurrent();

        void addBlock(const std::shared_ptr<Block>& block);
        int getBlockIndexForName(const std::string& name) const;
        std::shared_ptr<Block> getBlock(int index) const;
        std::shared_ptr<Block> getBlock(const std::string& name) const;
        int getBlockCount() const;

        bool isSelected() const;
        void select();

       protected:
        BlockPack();
        static std::shared_ptr<BlockPack> registerBlockPack(
            const std::shared_ptr<BlockPack> pack);

       private:
        bool selected;
        std::string name;
        std::vector<std::shared_ptr<Block> > blockVec;
        static std::vector<std::shared_ptr<BlockPack> > blockPacks;
};
}  // namespace planet
#endif
#pragma once
#ifndef WORLD_BLOCKPACK_HPP
#define WORLD_BLOCKPACK_HPP
#include <memory>
#include <vector>

#include "BlockInfoCollection.hpp"

namespace ofxPlanet {
class Block;
/**
 * BlockPack is list a block collection.
 */
class BlockPack {
       public:
		/**
		 * add BlockPack by specific block collection.
		 * @param blockInfoCollection
		 * @return
		 */
        static std::shared_ptr<BlockPack> load(
            const BlockInfoCollection& blockInfoCollection);
		/**
		 * add BlockPack.
		 * @paam name
		 * @return
		 */
        static std::shared_ptr<BlockPack> make(const std::string& name);
		/**
		 * returns a selecting TexturePack.
		 * @return
		 */
        static std::shared_ptr<BlockPack> getCurrent();

		/**
		 * add Block.
		 * @param block
		 */
        void addBlock(const std::shared_ptr<Block>& block);

		/**
		 * find block index from name.
		 * @param name
		 * @return
		 */
        int getBlockIndexForName(const std::string& name) const;

		/**
		 * returns block from index.
		 * @param index
		 * @return
		 */
        std::shared_ptr<Block> getBlock(int index) const;

		/**
		 * returns block from name.
		 * @param name
		 * @return
		 */
        std::shared_ptr<Block> getBlock(const std::string& name) const;

		/**
		 * returns count of blocks.
		 * @return
		 */
        int getBlockCount() const;

		/**
		 * return true if selected.
		 * @return
		 */
        bool isSelected() const;
		/**
		 * select this BlockPack.
		 */
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
}  // namespace ofxPlanet
#endif
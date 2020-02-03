#include "BlockPack.hpp"

#include "Block.hpp"

namespace planet {
std::vector<std::shared_ptr<BlockPack> > BlockPack::blockPacks;

std::shared_ptr<BlockPack> BlockPack::load(
    const BlockInfoCollection& blockInfoCollection) {
        auto tp = new BlockPack();
        auto ret = std::shared_ptr<BlockPack>(tp);
        for (int i = 0; i < blockInfoCollection.getBlockInfoCount(); i++) {
                BlockInfo info = blockInfoCollection.getBlockInfo(i);
                auto block =
                    std::make_shared<Block>(stringToBlockShape(info.shape),
                                            info.reference, info.textue, i);
                ret->addBlock(block);
        }
        return registerBlockPack(ret);
}
std::shared_ptr<BlockPack> BlockPack::make(const std::string& name) {
        auto tp = new BlockPack();
        tp->name = name;
        return registerBlockPack(std::shared_ptr<BlockPack>(tp));
}
std::shared_ptr<BlockPack> BlockPack::getCurrent() {
        for (auto tp : blockPacks) {
                if (tp->isSelected()) {
                        return tp;
                }
        }
        return nullptr;
}
void BlockPack::addBlock(const std::shared_ptr<Block>& block) {
        this->blockVec.emplace_back(block);
}
int BlockPack::getBlockIndexForName(const std::string& name) const {
        int i = 0;
        for (std::shared_ptr<Block> block : blockVec) {
                if (block->getName() == name) {
                        return i;
                }
                ++i;
        }
        return -1;
}
std::shared_ptr<Block> BlockPack::getBlock(int index) const {
        return this->blockVec.at(index);
}
std::shared_ptr<Block> BlockPack::getBlock(const std::string& name) const {
        return getBlock(this->getBlockIndexForName(name));
}
int BlockPack::getBlockCount() const {
        return static_cast<int>(this->blockVec.size());
}
bool BlockPack::isSelected() const { return this->selected; }
void BlockPack::select() {
        if (selected) {
                return;
        }
        for (auto tp : BlockPack::blockPacks) {
                tp->selected = false;
        }
        this->selected = true;
}
BlockPack::BlockPack() : selected(false), name(), blockVec() {}
std::shared_ptr<BlockPack> BlockPack::registerBlockPack(
    const std::shared_ptr<BlockPack> pack) {
        bool empty = blockPacks.empty();
        blockPacks.emplace_back(pack);
        if (empty) {
                pack->select();
        }
        return pack;
}
}  // namespace planet
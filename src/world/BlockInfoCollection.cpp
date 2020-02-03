#include "BlockInfoCollection.hpp"

#include "picojson/picojson.h"

namespace planet {
BlockInfoCollection::BlockInfoCollection() : blockInfoVec() {}
std::string BlockInfoCollection::serialize() const {
        picojson::object rootO;
        picojson::array blocksA;
        for (auto blockInfo : blockInfoVec) {
                picojson::object blockO;
                blockO["texture"] = picojson::value(blockInfo.textue);
                blockO["reference"] = picojson::value(blockInfo.reference);
                blockO["shape"] = picojson::value(blockInfo.shape);
                blocksA.push_back(picojson::value(blockO));
        }
        rootO["blocks"] = picojson::value(blocksA);
        return picojson::value(rootO).serialize(true);
}
void BlockInfoCollection::deserialize(const std::string& json) {
        picojson::value root;
        const std::string err = picojson::parse(root, json);
        if (err.empty() == false) {
                std::cerr << err << std::endl;
                return;
        }
        auto rootO = root.get<picojson::object>();
        auto blocksV = rootO["blocks"];
        auto blocksA = blocksV.get<picojson::array>();
        auto blocksIter = blocksA.begin();
        while (blocksIter != blocksA.end()) {
                BlockInfo bi;
                auto blockV = *blocksIter;
                auto blockO = blockV.get<picojson::object>();
                auto textureV = blockO["texture"];
                auto referenceV = blockO["reference"];
                // Shape属性は必須ではないのでデフォルト値がある
                std::string shape("Block");
                if (blockO.count("shape")) {
                        shape = blockO["shape"].get<std::string>();
                }
                bi.textue = textureV.get<std::string>();
                bi.reference = referenceV.get<std::string>();
                bi.shape = shape;
                blockInfoVec.emplace_back(bi);
                ++blocksIter;
        }
}
void BlockInfoCollection::addBlockInfo(const BlockInfo& blockInfo) {
        blockInfoVec.emplace_back(blockInfo);
}
BlockInfo BlockInfoCollection::getBlockInfo(int index) const {
        return blockInfoVec.at(index);
}
bool BlockInfoCollection::tryGetBlockInfo(const std::string& reference,
                                          BlockInfo& outBlockInfo) {
        auto iter = blockInfoVec.begin();
        while (iter != blockInfoVec.end()) {
                auto v = *iter;
                if (v.reference == reference) {
                        outBlockInfo.reference = v.reference;
                        outBlockInfo.textue = v.textue;
                        return true;
                }
                ++iter;
        }
        return false;
}
int BlockInfoCollection::getBlockInfoCount() const {
        return static_cast<int>(blockInfoVec.size());
}
}  // namespace planet
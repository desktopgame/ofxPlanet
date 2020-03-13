#include "BlockInfoCollection.hpp"

#include <ofJson.h>

namespace ofxPlanet {
BlockInfoCollection::BlockInfoCollection() : blockInfoVec() {}
std::string BlockInfoCollection::serialize() const {
		ofJson rootO;
		ofJson blocksA;
        for (auto blockInfo : blockInfoVec) {
				ofJson blockO;
                blockO["texture"] = blockInfo.textue;
                blockO["reference"] = blockInfo.reference;
                blockO["shape"] = blockInfo.shape;
				blocksA.push_back(blockO);
        }
        rootO["blocks"] = blocksA;
		return rootO.dump();
}
void BlockInfoCollection::deserialize(const std::string& json) {
        ofJson root = ofJson::parse(json);
		auto rootO = root;
        auto blocksV = rootO["blocks"];
		auto blocksA = blocksV;
        auto blocksIter = blocksA.begin();
        while (blocksIter != blocksA.end()) {
                BlockInfo bi;
                auto blockV = *blocksIter;
                auto blockO = blockV;
                auto textureV = blockO["texture"];
                auto referenceV = blockO["reference"];
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
BlockInfo::BlockInfo() : textue(), reference(), shape() {}
}  // namespace ofxPlanet
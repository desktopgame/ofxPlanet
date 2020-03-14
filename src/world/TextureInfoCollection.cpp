#include "TextureInfoCollection.hpp"

#include <ofFileUtils.h>
#include <ofUtils.h>

#include <ofJson.h>

namespace ofxPlanet {
TextureInfoCollection::TextureInfoCollection(const std::string& baseDirectory)
    : baseDirectory(baseDirectory), textureInfoVec() {}
TextureInfoCollection::TextureInfoCollection() : TextureInfoCollection(".") {}
std::string TextureInfoCollection::serialize() const {
        ofJson rootO;
        ofJson texturesA;
        for (auto texInfo : textureInfoVec) {
                ofJson textureO;
                ofJson mappingRuleO;
                textureO["baseFileName"] = texInfo.baseFileName;
                textureO["reference"] = texInfo.reference;
                if (texInfo.mappingRule.all.hasValue()) {
                        mappingRuleO["all"] = texInfo.mappingRule.all.getValue();
                }
                // front, back
                if (texInfo.mappingRule.front.hasValue()) {
                        mappingRuleO["front"] = texInfo.mappingRule.front.getValue();
                }
                if (texInfo.mappingRule.back.hasValue()) {
                        mappingRuleO["back"] = texInfo.mappingRule.back.getValue();
                }
                // left, right
                if (texInfo.mappingRule.left.hasValue()) {
                        mappingRuleO["left"] = texInfo.mappingRule.left.getValue();
                }
                if (texInfo.mappingRule.right.hasValue()) {
                        mappingRuleO["right"] = texInfo.mappingRule.right.getValue();
                }
                // top, bottom
                if (texInfo.mappingRule.top.hasValue()) {
                        mappingRuleO["top"] = texInfo.mappingRule.top.getValue();
                }
                if (texInfo.mappingRule.bottom.hasValue()) {
                        mappingRuleO["bottom"] = texInfo.mappingRule.bottom.getValue();
                }
                textureO["mappingRule"] = mappingRuleO;
                texturesA.push_back(textureO);
        }
        rootO["textures"] = texturesA;
        rootO["baseDirectory"] = this->baseDirectory;
        return rootO.dump();
}
void TextureInfoCollection::deserialize(const std::string& json) {
		ofJson root;
		std::stringstream(json) >> root;
		auto rootO = root;
        auto texturesV = rootO["textures"];
        auto texturesO = texturesV;
        auto baseDirV = rootO["baseDirectory"];

        auto texturesIter = texturesO.begin();
        while (texturesIter != texturesO.end()) {
                TextureInfo info;
                auto textureV = *texturesIter;
                auto textureO = textureV;
                auto baseFileNameV = textureO["baseFileName"];
                auto referenceV = textureO["reference"];
                auto mappingRuleV = textureO["mappingRule"];
                auto mappingRuleO = mappingRuleV;
                info.baseFileName = baseFileNameV.get<std::string>();
                info.reference = referenceV.get<std::string>();
				auto mappingRuleIter = mappingRuleO.items().begin();
                while (mappingRuleIter != mappingRuleO.items().end()) {
                        auto kv = *mappingRuleIter;
						auto key = kv.key();
						auto val = kv.value();
                        auto valS = val.get<std::string>();
                        if (key == "all") {
                                info.mappingRule.all.setValue(valS);
                        } else if (key == "top") {
                                info.mappingRule.top.setValue(valS);
                        } else if (key == "bottom") {
                                info.mappingRule.bottom.setValue(valS);
                        } else if (key == "left") {
                                info.mappingRule.left.setValue(valS);
                        } else if (key == "right") {
                                info.mappingRule.right.setValue(valS);
                        } else if (key == "front") {
                                info.mappingRule.front.setValue(valS);
                        } else if (key == "back") {
                                info.mappingRule.back.setValue(valS);
                        }
                        ++mappingRuleIter;
                }
                addTextureInfo(info);
                ++texturesIter;
        }
        this->baseDirectory = baseDirV.get<std::string>();
}
void TextureInfoCollection::addTextureInfo(const TextureInfo& textureInfo) {
        textureInfoVec.emplace_back(textureInfo);
}
TextureInfo TextureInfoCollection::getTextureInfo(int index) const {
        return textureInfoVec.at(index);
}
bool TextureInfoCollection::tryGetTextureInfo(const std::string& reference,
                                              TextureInfo& outTextureInfo) {
        auto iter = textureInfoVec.begin();
        while (iter != textureInfoVec.end()) {
                auto v = *iter;
                if (v.reference == reference) {
                        outTextureInfo.baseFileName = v.baseFileName;
                        outTextureInfo.mappingRule = v.mappingRule;
                        outTextureInfo.reference = v.reference;
                        return true;
                }
                ++iter;
        }
        return false;
}
int TextureInfoCollection::getTextureInfoCount() const {
        return static_cast<int>(textureInfoVec.size());
}
std::string TextureInfoCollection::getBaseDirectory() const {
        return this->baseDirectory;
}
std::string TextureInfoCollection::getBasePath() const {
        return ofToDataPath(baseDirectory);
}
TextureMappingRule::TextureMappingRule()
    : all(), top(), bottom(), left(), right(), front(), back() {}
TextureInfo::TextureInfo() : baseFileName(), reference(), mappingRule() {}
}  // namespace ofxPlanet
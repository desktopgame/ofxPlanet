#include "TextureInfoCollection.hpp"

#include <ofFileUtils.h>
#include <ofUtils.h>

#include "picojson/picojson.h"

namespace planet {
TextureInfoCollection::TextureInfoCollection(const std::string& baseDirectory)
    : baseDirectory(baseDirectory), textureInfoVec() {}
TextureInfoCollection::TextureInfoCollection() : TextureInfoCollection(".") {}
std::string TextureInfoCollection::serialize() const {
        picojson::object rootO;
        picojson::array texturesA;
        for (auto texInfo : textureInfoVec) {
                picojson::object textureO;
                picojson::object mappingRuleO;
                textureO["baseFileName"] =
                    picojson::value(texInfo.baseFileName);
                textureO["reference"] = picojson::value(texInfo.reference);
                if (texInfo.mappingRule.all.hasValue()) {
                        mappingRuleO["all"] =
                            picojson::value(texInfo.mappingRule.all.getValue());
                }
                // front, back
                if (texInfo.mappingRule.front.hasValue()) {
                        mappingRuleO["front"] = picojson::value(
                            texInfo.mappingRule.front.getValue());
                }
                if (texInfo.mappingRule.back.hasValue()) {
                        mappingRuleO["back"] = picojson::value(
                            texInfo.mappingRule.back.getValue());
                }
                // left, right
                if (texInfo.mappingRule.left.hasValue()) {
                        mappingRuleO["left"] = picojson::value(
                            texInfo.mappingRule.left.getValue());
                }
                if (texInfo.mappingRule.right.hasValue()) {
                        mappingRuleO["right"] = picojson::value(
                            texInfo.mappingRule.right.getValue());
                }
                // top, bottom
                if (texInfo.mappingRule.top.hasValue()) {
                        mappingRuleO["top"] =
                            picojson::value(texInfo.mappingRule.top.getValue());
                }
                if (texInfo.mappingRule.bottom.hasValue()) {
                        mappingRuleO["bottom"] = picojson::value(
                            texInfo.mappingRule.bottom.getValue());
                }
                textureO["mappingRule"] = picojson::value(mappingRuleO);
                texturesA.push_back(picojson::value(textureO));
        }
        rootO["textures"] = picojson::value(texturesA);
        rootO["baseDirectory"] = picojson::value(this->baseDirectory);
        return picojson::value(rootO).serialize(true);
}
void TextureInfoCollection::deserialize(const std::string& json) {
        picojson::value root;
        const std::string err = picojson::parse(root, json);
        if (err.empty() == false) {
                std::cerr << err << std::endl;
                return;
        }
        auto rootO = root.get<picojson::object>();
        auto texturesV = rootO["textures"];
        auto texturesO = texturesV.get<picojson::array>();
        auto baseDirV = rootO["baseDirectory"];

        auto texturesIter = texturesO.begin();
        while (texturesIter != texturesO.end()) {
                TextureInfo info;
                auto textureV = *texturesIter;
                auto textureO = textureV.get<picojson::object>();
                auto baseFileNameV = textureO["baseFileName"];
                auto referenceV = textureO["reference"];
                auto mappingRuleV = textureO["mappingRule"];
                auto mappingRuleO = mappingRuleV.get<picojson::object>();
                info.baseFileName = baseFileNameV.get<std::string>();
                info.reference = referenceV.get<std::string>();
                auto mappingRuleIter = mappingRuleO.begin();
                while (mappingRuleIter != mappingRuleO.end()) {
                        auto kv = *mappingRuleIter;
                        auto key = kv.first;
                        auto val = kv.second;
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
}  // namespace planet
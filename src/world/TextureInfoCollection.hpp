#pragma once
#ifndef WORLD_TEXTURECOLLECTIONINFO_HPP
#define WORLD_TEXTURECOLLECTIONINFO_HPP
#include <string>
#include <vector>

#include "Optional.hpp"

namespace ofxPlanet {
/**
 * TextureMappingRule is sides in block mapped texture.
 */
struct TextureMappingRule {
        explicit TextureMappingRule() = default;
        Optional<std::string> all, top, bottom, left, right, front, back;
};
/**
 * TextureInfo is texture information for one blocks.
 */
struct TextureInfo {
        explicit TextureInfo() = default;
        std::string baseFileName;
        std::string reference;
        TextureMappingRule mappingRule;
};
/**
 * TextureInfoCollection is collection of TextureInfo.
 */
class TextureInfoCollection {
       public:
        explicit TextureInfoCollection(const std::string& baseDirectory);
        explicit TextureInfoCollection();

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
         * add TextureInfo.
         * @param textureInfo
         */
        void addTextureInfo(const TextureInfo& textureInfo);

        /**
         * returns a TextureInfor from index.
         * @param index
         * @return
         */
        TextureInfo getTextureInfo(int index) const;

        /**
         * return true if found TextureInfo from reference.
         * @param reference
         * @param outTextureInfo
         * @return
         */
        bool tryGetTextureInfo(const std::string& reference,
                               TextureInfo& outTextureInfo);
        /**
         * returns a count all TextureInfo.
         * @return
         */
        int getTextureInfoCount() const;

        /**
         * returns a baseDirectory using for texture load.
         * @return
         */
        std::string getBaseDirectory() const;

        /**
         * returns a baseDirectory as absolute path using for texture load.
         * @return
         */
        std::string getBasePath() const;

       private:
        std::vector<TextureInfo> textureInfoVec;
        std::string baseDirectory;
};
}  // namespace ofxPlanet
#endif
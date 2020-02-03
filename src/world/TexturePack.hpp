#pragma once
#ifndef WORLD_TEXTUREPACK_HPP
#define WORLD_TEXTUREPACK_HPP
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Texture.hpp"
#include "TextureInfoCollection.hpp"
#include "TextureSet.hpp"

namespace ofxPlanet {

class Plane;
/**
 * TexturePack is list a texture collection.
 * using for exchange for block textures.
 */
class TexturePack : public std::enable_shared_from_this<TexturePack> {
       public:
        /**
         * add TexturePack by specific texture collection.
         * @param textureInfoCollection
         */
        static std::shared_ptr<TexturePack> load(
            const TextureInfoCollection& textureInfoCollection);
        /**
         * add TexturePack.
         * @param baseDirectory
         * @param name
         */
        static std::shared_ptr<TexturePack> make(
            const std::string& baseDirectory, const std::string& name);
        /**
         * returns a selecting TexturePack.
         * @return
         */
        static std::shared_ptr<TexturePack> getCurrent();

        /**
         * add TextureSet.
         * @param set
         */
        void addTextureSet(const TextureSet& set);

        /**
         * find texture index from name.
         * @param name
         * @return
         */
        int getTextureSetIndexForName(const std::string& name) const;

        /**
         * returns texture from index.
         * @param index
         * @return
         */
        const TextureSet& getTextureSet(int index) const;

        /**
         * remove a texture for specific index.
         */
        void removeTextureSet(int index);

        /**
         * returns count a textures.
         * @return
         */
        int getTextureSetCount() const;

        /**
         * returns texture from name.
         * @param name
         * @return
         */
        std::shared_ptr<Texture> getImage(const std::string& name) const;

        /**
         * return true if selected.
         */
        bool isSelected() const;
        /**
         * select this TexturePack.
         */
        void select();
        /**
         * resolve a mapping for block and texture.
         */
        void resolve();

        /**
         * returns a baseDirectory for using load texture.
         * @return
         */
        std::string getBaseDirectory() const;
        /**
         * returns a baseDirectory as absolute path for using load texture.
         */
        std::string getBasePath() const;

       protected:
        explicit TexturePack(const std::string& baseDirectory);
        static std::shared_ptr<TexturePack> registerTexturePack(
            const std::shared_ptr<TexturePack> pack);

       private:
        bool selected;
        std::string baseDirectory;
        std::string name;
        std::vector<TextureSet> textureSets;
        std::unordered_map<std::string, std::shared_ptr<Texture> > images;
        static std::vector<std::shared_ptr<TexturePack> > texturePacks;
};
}  // namespace ofxPlanet
#endif
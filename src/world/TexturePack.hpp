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

namespace planet {

class Plane;
class TexturePack : public std::enable_shared_from_this<TexturePack> {
       public:
        static std::shared_ptr<TexturePack> load(
            const TextureInfoCollection& textureInfoCollection);
        static std::shared_ptr<TexturePack> make(
            const std::string& baseDirectory, const std::string& name);
        static std::shared_ptr<TexturePack> getCurrent();

        void addTextureSet(const TextureSet& set);
        int getTextureSetIndexForName(const std::string& name) const;
        const TextureSet& getTextureSet(int index) const;
        void removeTextureSet(int index);
        int getTextureSetCount() const;

        std::shared_ptr<Texture> getImage(const std::string& name) const;

        bool isSelected() const;
        void select();
        void resolve();

        std::string getBaseDirectory() const;
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
}  // namespace planet
#endif
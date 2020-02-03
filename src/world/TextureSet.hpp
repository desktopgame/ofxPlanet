#pragma once
#ifndef WORLD_TEXTURESET_HPP
#define WORLD_TEXTURESET_HPP
#include <memory>

#include "Texture.hpp"
namespace planet {

class TexturePack;
class Texture;
class TextureSet {
       public:
        explicit TextureSet(const std::weak_ptr<const TexturePack> pack,
                            std::string name);
        std::string getName() const;

        std::shared_ptr<Texture> getTopImage() const;

        std::shared_ptr<Texture> getBottomImage() const;

        std::shared_ptr<Texture> getFrontImage() const;

        std::shared_ptr<Texture> getBackImage() const;

        std::shared_ptr<Texture> getLeftImage() const;

        std::shared_ptr<Texture> getRightImage() const;

        std::string top, bottom, front, back, left, right;

       private:
        std::weak_ptr<const TexturePack> pack;
        std::string name;
};
}  // namespace planet
#endif
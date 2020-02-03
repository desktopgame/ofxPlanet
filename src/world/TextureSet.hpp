#pragma once
#ifndef WORLD_TEXTURESET_HPP
#define WORLD_TEXTURESET_HPP
#include <memory>

#include "Texture.hpp"
namespace ofxPlanet {

class TexturePack;
class Texture;
/**
 * TextureSet is textures for side from blocks.
 */
class TextureSet {
       public:
        explicit TextureSet(const std::weak_ptr<const TexturePack> pack,
                            std::string name);
        /**
         * returns a texture name.
         * @return
         */
        std::string getName() const;

        /**
         * return a top-side texture.
         * @return
         */
        std::shared_ptr<Texture> getTopImage() const;

        /**
         * return a bottom side texture.
         * @return
         */
        std::shared_ptr<Texture> getBottomImage() const;

        /**
         * return a front side texture.
         * @return
         */
        std::shared_ptr<Texture> getFrontImage() const;

        /**
         * return a back side texture.
         */
        std::shared_ptr<Texture> getBackImage() const;

        /**
         * return a left side texture.
         */
        std::shared_ptr<Texture> getLeftImage() const;

        /**
         * return a right side texture.
         */
        std::shared_ptr<Texture> getRightImage() const;

        std::string top, bottom, front, back, left, right;

       private:
        std::weak_ptr<const TexturePack> pack;
        std::string name;
};
}  // namespace ofxPlanet
#endif
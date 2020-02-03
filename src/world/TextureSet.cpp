#include "TextureSet.hpp"

#include "Texture.hpp"
#include "TexturePack.hpp"
namespace planet {

TextureSet::TextureSet(const std::weak_ptr<const TexturePack> pack,
                       std::string name)
    : pack(pack),
      name(name),
      top(),
      bottom(),
      front(),
      back(),
      left(),
      right() {}

std::string TextureSet::getName() const { return name; }

std::shared_ptr<Texture> TextureSet::getTopImage() const {
        std::shared_ptr<TexturePack> tp =
            std::const_pointer_cast<TexturePack>(pack.lock());
        return tp->getImage(top);
}
std::shared_ptr<Texture> TextureSet::getBottomImage() const {
        std::shared_ptr<TexturePack> tp =
            std::const_pointer_cast<TexturePack>(pack.lock());
        return tp->getImage(bottom);
}

std::shared_ptr<Texture> TextureSet::getFrontImage() const {
        std::shared_ptr<TexturePack> tp =
            std::const_pointer_cast<TexturePack>(pack.lock());
        return tp->getImage(front);
}

std::shared_ptr<Texture> TextureSet::getBackImage() const {
        std::shared_ptr<TexturePack> tp =
            std::const_pointer_cast<TexturePack>(pack.lock());
        return tp->getImage(back);
}

std::shared_ptr<Texture> TextureSet::getLeftImage() const {
        std::shared_ptr<TexturePack> tp =
            std::const_pointer_cast<TexturePack>(pack.lock());
        return tp->getImage(left);
}

std::shared_ptr<Texture> TextureSet::getRightImage() const {
        std::shared_ptr<TexturePack> tp =
            std::const_pointer_cast<TexturePack>(pack.lock());
        return tp->getImage(right);
}
}  // namespace planet

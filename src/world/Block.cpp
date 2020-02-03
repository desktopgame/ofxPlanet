#include "Block.hpp"

#include "../common/GLM.hpp"
#include "BlockRenderer.hpp"
#include "Texture.hpp"
#include "TexturePack.hpp"
#include "World.hpp"
namespace planet {
Block::Block(BlockShape shape, const std::string& name,
             const std::string& textureReference, int id)
    : shape(shape), name(name), textureReference(textureReference), id(id) {}
void Block::batch(std::shared_ptr<World> world, BlockRenderer& renderer, int x,
                  int y, int z) {
        TextureSet set = getTextureSet();
        std::reference_wrapper<GraphicsRenderer> target =
            renderer.getCubeRenderer();
        if (this->shape == BlockShape::LeftSlab) {
                target = renderer.getLeftSlabRenderer();
        } else if (this->shape == BlockShape::RightSlab) {
                target = renderer.getRightSlabRenderer();
        } else if (this->shape == BlockShape::FrontSlab) {
                target = renderer.getFrontSlabRenderer();
        } else if (this->shape == BlockShape::BackSlab) {
                target = renderer.getBackSlabRenderer();
        } else if (this->shape == BlockShape::TopSlab) {
                target = renderer.getTopSlabRenderer();
        } else if (this->shape == BlockShape::BottomSlab) {
                target = renderer.getBottomSlabRenderer();
        }
        if (!world->isFilled(x - 1, y, z)) {
                target.get().putLeft(set.getLeftImage()->getName(), x, y, z);
        }
        if (!world->isFilled(x + 1, y, z)) {
                target.get().putRight(set.getRightImage()->getName(), x, y, z);
        }
        if (!world->isFilled(x, y, z - 1)) {
                target.get().putBack(set.getBackImage()->getName(), x, y, z);
        }
        if (!world->isFilled(x, y, z + 1)) {
                target.get().putFront(set.getFrontImage()->getName(), x, y, z);
        }
        if (!world->isFilled(x, y + 1, z)) {
                target.get().putTop(set.getTopImage()->getName(), x, y, z);
        }
        if (!world->isFilled(x, y - 1, z)) {
                target.get().putBottom(set.getBottomImage()->getName(), x, y,
                                       z);
        }
}

TextureSet Block::getTextureSet() const {
        return TexturePack::getCurrent()->getTextureSet(textureSetIndex);
}

void Block::setTextureSetIndex(int textureSetIndex) {
        this->textureSetIndex = textureSetIndex;
}

int Block::getTextureSetIndex() const { return textureSetIndex; }

std::string Block::getTextureReference() const {
        return this->textureReference;
}

std::string Block::getName() const { return name; }

glm::vec3 Block::getSize() const { return sizeFromShape(shape); }

BlockShape Block::getShape() const { return shape; }

int Block::getID() const { return id; }

glm::vec3 sizeFromShape(BlockShape shape) {
        if (shape == BlockShape::Block) {
                return glm::vec3(2, 2, 2);
        } else if (shape == BlockShape::LeftSlab ||
                   shape == BlockShape::RightSlab) {
                return glm::vec3(1, 2, 2);
        } else if (shape == BlockShape::FrontSlab ||
                   shape == BlockShape::BackSlab) {
                return glm::vec3(2, 2, 1);
        } else if (shape == BlockShape::TopSlab ||
                   shape == BlockShape::BottomSlab) {
                return glm::vec3(2, 1, 2);
        }
        return glm::vec3(2, 2, 2);
}

BlockShape stringToBlockShape(const std::string& str) {
        if (str == "Block" || str == "BLOCK") {
                return BlockShape::Block;
        }
        if (str == "LeftSlab" || str == "LEFTSLAB") {
                return BlockShape::LeftSlab;
        }
        if (str == "RightSlab" || str == "RIGHTSLAB") {
                return BlockShape::RightSlab;
        }
        if (str == "FrontSlab" || str == "FRONTSLAB") {
                return BlockShape::FrontSlab;
        }
        if (str == "BackSlab" || str == "BACKSLAB") {
                return BlockShape::BackSlab;
        }
        if (str == "TopSlab" || str == "TOPSLAB") {
                return BlockShape::TopSlab;
        }
        if (str == "BottomSlab" || str == "BottomSLAB") {
                return BlockShape::BottomSlab;
        }
        return BlockShape::Block;
}
}  // namespace planet
#pragma once
#ifndef WORLD_BLOCK_HPP
#define WORLD_BLOCK_HPP
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>

#include "BlockShape.hpp"
#include "TextureSet.hpp"
namespace planet {

class World;
class BlockRenderer;
glm::vec3 sizeFromShape(BlockShape shape);
BlockShape stringToBlockShape(const std::string& str);

class Block {
       public:
        explicit Block(BlockShape shape, const std::string& name,
                       const std::string& textureReference, int id);
        virtual ~Block() = default;

        void batch(std::shared_ptr<World> world, BlockRenderer& renderer, int x,
                   int y, int z);

        TextureSet getTextureSet() const;

        void setTextureSetIndex(int textureSetIndex);
        int getTextureSetIndex() const;

        std::string getTextureReference() const;
        std::string getName() const;
        glm::vec3 getSize() const;
        BlockShape getShape() const;
        int getID() const;

       protected:
       private:
        BlockShape shape;
        std::string textureReference;
        std::string name;
        int id;
        int textureSetIndex;
};
}  // namespace planet
#endif
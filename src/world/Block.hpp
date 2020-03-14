#pragma once
#ifndef WORLD_BLOCK_HPP
#define WORLD_BLOCK_HPP
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>

#include "BlockShape.hpp"
#include "IWorld.hpp"
#include "TextureSet.hpp"
namespace ofxPlanet {

class BlockRenderer;
class IAreaBounds;
/**
 * @param shape
 * @return
 */
glm::vec3 sizeFromShape(BlockShape shape);
/**
 * @param str
 * @return
 */
BlockShape stringToBlockShape(const std::string& str);
/**
 * Block.
 */
class Block {
       public:
        explicit Block(BlockShape shape, const std::string& name,
                       const std::string& textureReference, int id);
        virtual ~Block() = default;
        /**
         * write position data to renderer.
         * @param areaBounds
         * @param brightness
         * @param renderer
         * @parma modelPosition
         * @param viewPosition
         */
        void batch(const IAreaBounds& areaBounds, BlockRenderer& renderer,
                   int brightness, const glm::ivec3& modelPosition,
                   const glm::ivec3& viewPosition);
        /**
         * returns TextureSet for rendering this block.
         * @return
         */
        TextureSet getTextureSet() const;

        /**
         * overwrite texture index.
         * @param textureSetIndex
         */
        void setTextureSetIndex(int textureSetIndex);

        /**
         * returns texture index.
         * @return
         */
        int getTextureSetIndex() const;

        /**
         * returns texture name.
         * @return
         */
        std::string getTextureReference() const;

        /**
         * returns block name.
         */
        std::string getName() const;

        /**
         * returns block size.
         */
        glm::vec3 getSize() const;

        /**
         * returns block shape.
         * @return
         */
        BlockShape getShape() const;

        /**
         * retunrs block id.
         * @return
         */
        int getID() const;

       protected:
       private:
        BlockShape shape;
        std::string textureReference;
        std::string name;
        int id;
        int textureSetIndex;
};
}  // namespace ofxPlanet
#endif
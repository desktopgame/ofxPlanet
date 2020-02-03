#pragma once
#ifndef WORLD_WORLD_HPP
#define WORLD_WORLD_HPP
#include <ofCamera.h>
#include <ofFbo.h>
#include <ofVec3f.h>

#include <array>
#include <memory>

#include "BlockRenderer.hpp"
#include "BlockTable.hpp"
namespace planet {

class Camera;
class World;
class WorldPart {
       public:
        explicit WorldPart(const std::shared_ptr<World>& world,
                           glm::ivec3 offset);

        std::shared_ptr<World> world;
        glm::ivec3 offset;
};

class Entity;
class Block;
class World : public std::enable_shared_from_this<World> {
       public:
        static std::shared_ptr<World> create(ofShader& shader,
                                             const glm::ivec3& size);
        static std::shared_ptr<World> create(ofShader& shader, int xSize,
                                             int ySize, int zSize);
        static int floatToInt(float f);

        void load(const BlockTable& table);
        void clear();

        void update();
        void drawToBuffer();
        void render();

        void invalidate();
        void rehash();

        void setBlock(glm::vec3 pos, std::shared_ptr<Block> block);
        void setBlock(glm::ivec3 pos, std::shared_ptr<Block> block);
        void setBlock(float x, float y, float z, std::shared_ptr<Block> block);
        void setBlock(int x, int y, int z, std::shared_ptr<Block> block);
        std::shared_ptr<Block> getBlock(int x, int y, int z) const;
        std::shared_ptr<Block> getBlock(float x, float y, float z) const;
        std::shared_ptr<Block> getBlock(glm::vec3 pos) const;
        std::shared_ptr<Block> getBlock(glm::ivec3 pos) const;

        bool isContains(int x, int y, int z) const;
        bool isContains(const glm::ivec3& v) const;
        bool isContains(const glm::vec3& v) const;
        bool isEmpty(int x, int y, int z) const;
        bool isFilled(int x, int y, int z) const;
        int getGroundY(int x, int z) const;

        glm::vec3 getPhysicalPosition(int x, int y, int z) const;

        int getXSize() const;
        int getYSize() const;
        int getZSize() const;
        glm::ivec3 getSize() const;

        void setPlayMode(bool playMode);
        bool isPlayMode() const;

        std::vector<WorldPart> split(int splitNum) const;

       private:
        void checkFBO();
        explicit World(ofShader& shader, const glm::ivec3& size);
        explicit World(ofShader& shader, int xSize, int ySize, int zSize);
        std::vector<std::vector<std::vector<std::shared_ptr<Block> > > > blocks;
        std::vector<glm::ivec3> notBlockPositionsVec;
        bool isInvalid;
        int xSize, ySize, zSize;
        int fboW, fboH;
        BlockRenderer renderer;
        ofFbo fbo;
        ofShader& shader;
        bool bIsPlayMode;
};

}  // namespace planet
#endif
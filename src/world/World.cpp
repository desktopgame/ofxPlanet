#include "World.hpp"

#include <ofAppRunner.h>
#include <ofEasyCam.h>
#include <ofFbo.h>
#include <ofLight.h>
#include <ofVec3f.h>

#include <algorithm>
#include <string>

#include "../game/ofApp.h"
#include "Block.hpp"
#include "BlockPack.hpp"
#include "BlockRenderer.hpp"
#include "Camera.hpp"
#include "TexturePack.hpp"
namespace planet {

WorldPart::WorldPart(const std::shared_ptr<World>& world, glm::ivec3 offset)
    : world(world), offset(offset) {}
std::shared_ptr<World> World::create(ofShader& shader, const glm::ivec3& size) {
        return create(shader, size.x, size.y, size.z);
}

std::shared_ptr<World> World::create(ofShader& shader, int xSize, int ySize,
                                     int zSize) {
        World* world = new World(shader, xSize, ySize, zSize);
        std::shared_ptr<World> ret = std::shared_ptr<World>(world);
        for (int i = 0; i < xSize; i++) {
                std::vector<std::vector<std::shared_ptr<Block> > > yline;
                for (int j = 0; j < ySize; j++) {
                        std::vector<std::shared_ptr<Block> > zline;
                        for (int k = 0; k < zSize; k++) {
                                zline.emplace_back(nullptr);
                        }
                        yline.emplace_back(zline);
                }
                ret->blocks.emplace_back(yline);
        }
        return ret;
}

int World::floatToInt(float f) { return static_cast<int>(std::round(f)); }

void World::load(const BlockTable& table) {
        auto bp = BlockPack::getCurrent();
        for (int x = 0; x < table.getXSize(); x++) {
                for (int y = 0; y < table.getYSize(); y++) {
                        for (int z = 0; z < table.getZSize(); z++) {
                                BlockPrefab i = table.get(x, y, z);
                                if (i.id == -1) {
                                        continue;
                                }
                                if (i.instanced) {
                                        setBlock(x, y, z, bp->getBlock(i.id));
                                } else {
                                        setBlock(x, y, z, bp->getBlock(i.id));
                                }
                        }
                }
        }
}

void World::clear() {
        for (int x = 0; x < xSize; x++) {
                for (int y = 0; y < ySize; y++) {
                        for (int z = 0; z < zSize; z++) {
                                setBlock(x, y, z, nullptr);
                        }
                }
        }
}

void World::update() {
        if (!isPlayMode()) {
                return;
        }
}

void World::drawToBuffer() {
        rehash();
        checkFBO();
        // screenBuffer.bind();
        fbo.begin();
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO,
                            GL_ONE);
        glEnable(GL_DEPTH_TEST);
        // ofClear(0);
        fbo.clear();
        ofEnableAlphaBlending();
        ofSetBackgroundColor(0, 0, 0, 0);
        renderer.render();
        fbo.end();
        // screenBuffer.unbind();
}

void World::render() {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
        // screenBuffer.render();
}

void World::invalidate() { this->isInvalid = true; }

void World::rehash() {
        if (!isInvalid) {
                return;
        }
        this->isInvalid = false;
        checkFBO();
        renderer.clear();
        auto w = std::const_pointer_cast<World>(shared_from_this());
        for (int x = 0; x < xSize; x++) {
                for (int y = 0; y < ySize; y++) {
                        for (int z = 0; z < zSize; z++) {
                                auto block = getBlock(x, y, z);
                                if (!block) {
                                        continue;
                                }
                                block->batch(w, renderer, x, y, z);
                        }
                }
        }
        renderer.update();
}

void World::setBlock(glm::vec3 pos, std::shared_ptr<Block> block) {
        setBlock(pos.x, pos.y, pos.z, block);
}

void World::setBlock(glm::ivec3 pos, std::shared_ptr<Block> block) {
        setBlock(pos.x, pos.y, pos.z, block);
}

void World::setBlock(float x, float y, float z, std::shared_ptr<Block> block) {
        setBlock(World::floatToInt(x), World::floatToInt(y),
                 World::floatToInt(z), block);
}

void World::setBlock(int x, int y, int z, std::shared_ptr<Block> block) {
        // 以前同じ座標にブロックが置かれていたなら削除
        glm::ivec3 pos(x, y, z);
        auto iter = std::remove_if(
            notBlockPositionsVec.begin(), notBlockPositionsVec.end(),
            [pos](glm::ivec3 e) -> bool { return e == pos; });
        notBlockPositionsVec.erase(iter, notBlockPositionsVec.end());
        //ブロックのサイズがデフォルトでないなら記録する
        if (block && block->getShape() != BlockShape::Block) {
                notBlockPositionsVec.emplace_back(pos);
        }
        blocks[x][y][z] = block;
        this->isInvalid = true;
}

std::shared_ptr<Block> World::getBlock(int x, int y, int z) const {
        return blocks[x][y][z];
}
std::shared_ptr<Block> World::getBlock(float x, float y, float z) const {
        return getBlock(World::floatToInt(x), World::floatToInt(y),
                        World::floatToInt(z));
}
std::shared_ptr<Block> World::getBlock(glm::vec3 pos) const {
        return getBlock(pos.x, pos.y, pos.z);
}
std::shared_ptr<Block> World::getBlock(glm::ivec3 pos) const {
        return getBlock(pos.x, pos.y, pos.z);
}

bool World::isContains(int x, int y, int z) const {
        if (x < 0 || x >= xSize) return false;
        if (y < 0 || y >= ySize) return false;
        if (z < 0 || z >= zSize) return false;
        return true;
}
bool World::isContains(const glm::ivec3& v) const {
        return isContains(v.x, v.y, v.z);
}
bool World::isContains(const glm::vec3& v) const {
        return isContains(floatToInt(v.x), floatToInt(v.y), floatToInt(v.z));
}
bool World::isEmpty(int x, int y, int z) const {
        if (!isContains(x, y, z)) {
                return true;
        }
        auto block = getBlock(x, y, z);
        return block == nullptr;
}
bool World::isFilled(int x, int y, int z) const {
        if (isEmpty(x, y, z)) {
                return false;
        }
        return getBlock(x, y, z)->getShape() == BlockShape::Block;
}
int World::getGroundY(int x, int z) const {
        for (int i = 0; i < ySize; i++) {
                if (!getBlock(x, i, z)) {
                        return i;
                }
        }
        return ySize;
}
glm::vec3 World::getPhysicalPosition(int x, int y, int z) const {
        // まずは全てのブロックが通常ブロックである前提で計算する
        glm::vec3 pos(x * 2, y * 2, z * 2);
        /*
        for (auto e : notBlockPositionsVec) {
                glm::vec3 size = glm::vec3(2, 2, 2) - getBlock(e.x, e.y,
        e.z)->getSize(); if (e.x <= x && e.y <= y && e.z <= z) { pos.x -=
        size.x; pos.y -= size.y; pos.z -= size.z;
                }
        }*/
        return pos;
}
int World::getXSize() const { return xSize; }
int World::getYSize() const { return ySize; }
int World::getZSize() const { return zSize; }
glm::ivec3 World::getSize() const { return glm::ivec3(xSize, ySize, zSize); }
void World::setPlayMode(bool playMode) { this->bIsPlayMode = playMode; }
bool World::isPlayMode() const { return bIsPlayMode; }

std::vector<WorldPart> World::split(int splitNum) const {
        int sx = xSize / splitNum;
        int sz = zSize / splitNum;
        std::vector<WorldPart> ret;
        for (int i = 0; i < splitNum; i++) {
                for (int j = 0; j < splitNum; j++) {
                        auto w =
                            World::create(shader, glm::ivec3(sx, ySize, sz));
                        for (int x = (sx * i); x < (sx * i) + sx; x++) {
                                for (int y = 0; y < ySize; y++) {
                                        for (int z = (sz * j);
                                             z < (sz * j) + sz; z++) {
                                                int ix = x - (sx * i);
                                                int iz = z - (sz * j);
                                                w->setBlock(
                                                    glm::ivec3(ix, y, iz),
                                                    this->getBlock(x, y, z));
                                        }
                                }
                        }
                        float xoffs = (sx * splitNum * 2) - ((sx * i) * 2);
                        float zoffs = ((sz * j) * 2);
                        ret.emplace_back(
                            WorldPart(w, glm::ivec3(xoffs, 0, zoffs)));
                }
        }
        return ret;
}

void World::checkFBO() {
        int newW = ofGetWidth();
        int newH = ofGetHeight();
        if (this->fboW != newW || this->fboH != newH) {
                fbo.allocate(newW, newH);
                this->fboW = newW;
                this->fboH = newH;
        }
}

// private
World::World(ofShader& shader, const glm::ivec3& size)
    : World(shader, size.x, size.y, size.z) {}

World::World(ofShader& shader, int xSize, int ySize, int zSize)
    : notBlockPositionsVec(),
      blocks(),
      xSize(xSize),
      ySize(ySize),
      zSize(zSize),
      renderer(*this, shader),
      isInvalid(true),
      bIsPlayMode(false),
      fbo(),
      shader(shader),
      fboW(-1),
      fboH(-1) {}
}  // namespace planet

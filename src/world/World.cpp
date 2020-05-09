#include "World.hpp"

#include <ofEvents.h>

#include <algorithm>
#include <iostream>

#include "Block.hpp"
#include "BlockPack.hpp"
#include "BlockTable.hpp"
#include "Chunk.hpp"
#include "LightTable.hpp"
#include "Sector.hpp"
#include "biome/Biome.hpp"

namespace ofxPlanet {
// Utility
World::Instance World::create(ofShader& shader, int worldYSize) {
        return Instance(new World(shader, worldYSize));
}
// IWorld
void World::computeBrightness() {
        for (auto sector : this->sectorVec) {
                sector->computeBrightness();
        }
}
int World::getYSize() const { return this->worldYSize; }
std::shared_ptr<Block> World::getBlock(int x, int y, int z) const {
        for (auto sector : this->sectorVec) {
                auto chunk = sector->getChunk();
                if (chunk->isContains(x, y, z)) {
                        x -= chunk->getXOffset();
                        z -= chunk->getZOffset();
                        return sector->getBlock(x, y, z);
                }
        }
        return nullptr;
}
bool World::isFilled(int x, int y, int z) const {
        if (y < 0) {
                return false;
        }
        auto block = getBlock(x, y, z);
        if (block == nullptr) {
                return false;
        }
        return block->getShape() == BlockShape::Block;
}
int World::getBrightness(int x, int y, int z) const {
        for (auto sector : this->sectorVec) {
                auto chunk = sector->getChunk();
                if (chunk->isContains(x, y, z)) {
                        x -= chunk->getXOffset();
                        z -= chunk->getZOffset();
                        return sector->getLightTable().getLight(x, y, z);
                }
        }
        return 15;
}
ofShader& World::getShader() { return this->shader; }
glm::vec3 World::getViewPosition() const { return this->viewPosition; }
int World::getViewRange() const { return this->viewRange; }
ChunkLoadStyle World::getChunkLoadStyle() const {
        return ChunkLoadStyle::VisibleChunk;
}
std::shared_ptr<Sector> World::getSector(int xOffset, int zOffset) const {
        for (auto sector : this->sectorVec) {
                auto chunk = sector->getChunk();
                if (chunk->getXOffset() == xOffset &&
                    chunk->getZOffset() == zOffset) {
                        return sector;
                }
        }
        return nullptr;
}
std::shared_ptr<Chunk> World::getCurrentChunk() { return this->currentChunk; }
// World
void World::invalidateBrightness() {
        for (auto sector : this->sectorVec) {
                sector->invalidateBrightness();
        }
}
void World::setViewPosition(const glm::vec3& viewPosition) {
        this->viewPosition = viewPosition;
        Chunk::Instance newChunk = nullptr;
        for (auto sector : sectorVec) {
                auto chunk = sector->getChunk();
                if (!sector->isGenerated()) {
                        continue;
                }
                newChunk =
                    chunk->lookup(glm::vec3(1, 0, 1) * this->viewPosition);
                if (newChunk) {
                        break;
                }
        }
        if (!newChunk) {
                newChunk = loadOrGenChunk(viewPosition.x, viewPosition.z);
        }
        if (this->currentChunk != newChunk) {
                this->currentChunk = newChunk;
                this->updateNeighborChunks();
                for (auto sector : sectorVec) sector->getChunk()->tidy();
        }
}
void World::setBiome(std::shared_ptr<Biome> biome) { this->biome = biome; }
std::shared_ptr<Biome> World::getBiome() const { return this->biome; }
void World::setLoadRange(int loadRange) { this->loadRange = loadRange; }
int World::getLoadRange() const { return this->loadRange; }
void World::setVisibleChunkCount(int visibleChunkCount) {
        this->visibleChunkCount = visibleChunkCount;
}
int World::getVisibleChunkCount() const { return this->visibleChunkCount; }
std::shared_ptr<Sector> World::getSectorAt(int index) const {
        return this->sectorVec.at(index);
}
int World::getSectorCount() const { return static_cast<int>(sectorVec.size()); }
std::shared_ptr<Chunk> World::findChunk(int x, int z) const {
        auto sector = findChunkImpl(x, z);
        if (sector == nullptr) {
                return nullptr;
        }
        return sector->getChunk();
}
std::shared_ptr<Chunk> World::loadChunk(int x, int z) {
        bool _;
        return loadChunk(x, z, _);
}
std::shared_ptr<Chunk> World::loadChunk(int x, int z, bool& isCreatedNewChunk) {
        auto sector = loadChunkImpl(x, z, isCreatedNewChunk);
        return sector->getChunk();
}
std::shared_ptr<Chunk> World::loadOrGenChunk(int x, int z) {
        return loadOrGenChunkImpl(x, z, 0, 0);
}
void World::draw() {
        for (auto sector : sectorVec) {
                sector->getChunk()->draw();
        }
}
// private
World::World(ofShader& shader, int worldYSize)
    : worldYSize(worldYSize),
      chunkXSize(32),
      chunkZSize(32),
      viewRange(64),
      loadRange(128),
      visibleChunkCount(6),
      viewPosition(0, 0, 0),
      sectorVec(),
      shader(shader),
      currentChunk(nullptr),
      biome(nullptr) {}
std::shared_ptr<Sector> World::findChunkImpl(int x, int z) const {
        int offsetX = computeChunkOffsetX(x);
        int offsetZ = computeChunkOffsetZ(z);
        for (auto sector : sectorVec) {
                auto chunk = sector->getChunk();
                if (chunk->isContains(x, 0, z) ||
                    (chunk->getXOffset() == offsetX &&
                     chunk->getZOffset() == offsetZ)) {
                        return sector;
                }
        }
        return nullptr;
}
std::shared_ptr<Sector> World::loadChunkImpl(int x, int z,
                                             bool& isCreatedNewChunk) {
        isCreatedNewChunk = false;
        auto sector = findChunkImpl(x, z);
        if (sector) {
                return sector;
        }
        isCreatedNewChunk = true;
        int offsetX = computeChunkOffsetX(x);
        int offsetZ = computeChunkOffsetZ(z);
        sector = std::make_shared<Sector>(*this, offsetX, offsetZ, chunkXSize,
                                          chunkZSize);
        this->sectorVec.emplace_back(sector);
        return sector;
}
std::shared_ptr<Chunk> World::loadOrGenChunkImpl(int x, int z, int xOffset,
                                                 int zOffset) {
        int offsetX = this->loadRange;
        int offsetZ = this->loadRange;
        for (int addX = offsetX; addX > 0; addX--) {
                int ax = addX;
                for (int addZ = offsetZ; addZ > 0; addZ--) {
                        int az = addZ;
                        loadOrGenChunkRange(x + ax, z - az, 0, 0);
                        loadOrGenChunkRange(x - ax, z + az, 0, 0);
                        loadOrGenChunkRange(x + ax, z + az, 0, 0);
                        loadOrGenChunkRange(x - ax, z - az, 0, 0);
                }
                loadOrGenChunkRange(x - ax, z, 0, 0);
                loadOrGenChunkRange(x, z - ax, 0, 0);
                loadOrGenChunkRange(x + ax, z, 0, 0);
                loadOrGenChunkRange(x, z + ax, 0, 0);
        }
        return loadOrGenChunkRange(x, z, 0, 0);
}
std::shared_ptr<Chunk> World::loadOrGenChunkRange(int x, int z, int xOffset,
                                                  int zOffset) {
        bool genCenter, genLeft, genRight, genTop, genBottom, genLTop, genRTop,
            genLBottom, genRBottom;
        auto centerSector = loadChunkImpl(x, z, genCenter);
        if (centerSector->isGenerated()) {
                return centerSector->getChunk();
        }
        int dx = computeGridX(x);
        int dz = computeGridZ(z);
        int cx = 2;
        int cz = 2;
        if (!(dx == cx && dz == cz)) {
                return nullptr;
        }

        auto leftSector = loadChunkImpl(x - chunkXSize, z, genLeft);
        auto leftTopSector =
            loadChunkImpl(x - chunkXSize, z - chunkZSize, genLTop);
        auto leftBottomSector =
            loadChunkImpl(x - chunkXSize, z + chunkZSize, genLBottom);

        auto rightSector = loadChunkImpl(x + chunkXSize, z, genRight);
        auto rightTopSector =
            loadChunkImpl(x + chunkXSize, z - chunkZSize, genRTop);
        auto rightBottomSector =
            loadChunkImpl(x + chunkXSize, z + chunkZSize, genRBottom);

        auto topSector = loadChunkImpl(x, z - chunkZSize, genTop);
        auto bottomSector = loadChunkImpl(x, z + chunkZSize, genBottom);
        auto bp = BlockPack::getCurrent();
        if (!centerSector->isGenerated() && !leftSector->isGenerated() &&
            !rightSector->isGenerated() && !topSector->isGenerated() &&
            !bottomSector->isGenerated() && !leftTopSector->isGenerated() &&
            !rightTopSector->isGenerated() &&
            !leftBottomSector->isGenerated() &&
            !rightBottomSector->isGenerated()) {
                BlockTable bt(chunkXSize * 3, this->worldYSize,
                              this->chunkZSize * 3);
                biome->generate(bt);
                for (int x = 0; x < chunkXSize; x++) {
                        for (int z = 0; z < chunkZSize; z++) {
                                for (int y = 0; y < this->worldYSize; y++) {
                                        leftTopSector->setBlock(
                                            x, y, z,
                                            bp->getBlock(
                                                bt.get(x, y, z).id));
                                        topSector->setBlock(
                                            x, y, z,
                                            bp->getBlock(
                                                bt.get((chunkXSize) + x, y,
                                                            z)
                                                    .id));
                                        rightTopSector->setBlock(
                                            x, y, z,
                                            bp->getBlock(
                                                bt.get(
                                                      (chunkXSize * 2) + x, y,
                                                      z)
                                                    .id));
                                        //--
                                        leftSector->setBlock(
                                            x, y, z,
                                            bp->getBlock(
                                                bt.get(x, y,
                                                            z + chunkZSize)
                                                    .id));
                                        centerSector->setBlock(
                                            x, y, z,
                                            bp->getBlock(
                                                bt.get((chunkXSize) + x, y,
                                                            z + chunkZSize)
                                                    .id));
                                        rightSector->setBlock(
                                            x, y, z,
                                            bp->getBlock(
                                                bt.get(
                                                      (chunkXSize * 2) + x, y,
                                                      z + chunkZSize)
                                                    .id));
                                        //--
                                        leftBottomSector->setBlock(
                                            x, y, z,
                                            bp->getBlock(
                                                bt.get(
                                                      x, y,
                                                      z + (chunkZSize * 2))
                                                    .id));
                                        bottomSector->setBlock(
                                            x, y, z,
                                            bp->getBlock(
                                                bt.get(
                                                      (chunkXSize) + x, y,
                                                      z + (chunkZSize * 2))
                                                    .id));
                                        rightBottomSector->setBlock(
                                            x, y, z,
                                            bp->getBlock(
                                                bt.get(
                                                      (chunkXSize * 2) + x, y,
                                                      z + (chunkZSize * 2))
                                                    .id));
                                }
                        }
                }
                leftTopSector->setGenerated(true);
                topSector->setGenerated(true);
                rightTopSector->setGenerated(true);

                leftSector->setGenerated(true);
                centerSector->setGenerated(true);
                rightSector->setGenerated(true);

                leftBottomSector->setGenerated(true);
                bottomSector->setGenerated(true);
                rightBottomSector->setGenerated(true);

                return centerSector->getChunk();
        }
        return nullptr;
}
int World::computeChunkOffsetX(int x) const {
        int xOffset = (x / chunkXSize) * chunkXSize;
        if (x < 0) {
                int a = ((x % chunkXSize) / chunkXSize) - 1;
                xOffset += (a * chunkXSize);
        }
        return xOffset;
}
int World::computeChunkOffsetZ(int z) const {
        int zOffset = (z / chunkZSize) * chunkZSize;
        if (z < 0) {
                int a = ((z % chunkZSize) / chunkZSize) - 1;
                zOffset += (a * chunkZSize);
        }
        return zOffset;
}
void World::updateNeighborChunks() {
        int visibleChunks = 0;
        for (auto sector : sectorVec) sector->getChunk()->hide();
        if (this->currentChunk == nullptr) {
                return;
        }
        for (auto sector : sectorVec) {
                auto chunk = sector->getChunk();
                auto neighbor =
                    chunk->getNeighbor(this->currentChunk, this->viewRange);
                for (auto nc : neighbor) {
                        nc->setVisible(true);
                        visibleChunks++;
                }
        }
        if (visibleChunks < this->visibleChunkCount) {
                loadOrGenChunk(viewPosition.x, viewPosition.z);
        }
}
int World::computeGridX(int x) const {
        int ox = x;
        int dx = 0;
        if (x >= 0) {
                dx = ((ox / this->chunkXSize) % 3);
                dx++;
        } else {
                dx = 4 - computeGridX(-x);
        }
        return dx;
}
int World::computeGridZ(int z) const {
        int oz = z;
        int dz = 0;

        if (z >= 0) {
                dz = ((oz / this->chunkZSize) % 3);
                dz++;
        } else {
                dz = 4 - computeGridZ(-z);
        }
        return dz;
}
}  // namespace ofxPlanet
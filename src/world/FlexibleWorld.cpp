#include "FlexibleWorld.hpp"
#include "Chunk.hpp"
#include "biome/Biome.hpp"
#include "BlockTable.hpp"
#include "BlockPack.hpp"
#include "Block.hpp"
#include <algorithm>
#include <iostream>

namespace ofxPlanet {
// FlexibleChunk
namespace detail {
FlexibleChunk::FlexibleChunk(IWorld & world, int xOffset, int zOffset, int xSize, int zSize)
 : table(), chunk(Chunk::create(world, xOffset, zOffset, xSize, zSize)) {
	for (int x = 0; x < xSize; x++) {
		std::vector < std::vector<std::shared_ptr<Block> > > grid;
		for (int y = 0; y < world.getYSize(); y++) {
			std::vector<std::shared_ptr<Block> > line;
			for (int z = 0; z < xSize; z++) {
				line.emplace_back(nullptr);
			}
			grid.emplace_back(line);
		}
		table.emplace_back(grid);
	}
}
}
// FlexibleChunkOffset
FlexibleChunkOffset::FlexibleChunkOffset(int x, int z) : x(x), z(z) {
}
FlexibleChunkOffset::FlexibleChunkOffset() : FlexibleChunkOffset(0,0) {
}
// Utility
FlexibleWorld::Instance FlexibleWorld::create(ofShader & shader, int worldYSize) {
	return Instance(new FlexibleWorld(shader, worldYSize));
}
// IWorld
void FlexibleWorld::computeBrightness() {
}
int FlexibleWorld::getYSize() const {
	return this->worldYSize;
}
std::shared_ptr<Block> FlexibleWorld::getBlock(int x, int y, int z) const {
	for (auto fc : this->chunkVec) {
		if (fc->chunk->isContains(x, y, z)) {
			x -= fc->chunk->getXOffset();
			z -= fc->chunk->getZOffset();
			return fc->table[x][y][z];
		}
	}
	return nullptr;
}
bool FlexibleWorld::isFilled(int x, int y, int z) const {
	if (x < 0 || y < 0 || z < 0) {
		return false;
	}
	auto block = getBlock(x, y, z);
	if (block == nullptr) {
		return false;
	}
	return block->getShape() == BlockShape::Block;
}
ofShader & FlexibleWorld::getShader() {
	return this->shader;
}
glm::vec3 FlexibleWorld::getViewPosition() const {
	return this->viewPosition;
}
int FlexibleWorld::getViewRange() const {
	return this->viewRange;
}
ChunkLoadStyle FlexibleWorld::getChunkLoadStyle() const {
	return ChunkLoadStyle::VisibleChunk;
}
LightTable & FlexibleWorld::getLightTable() {
	return this->lightTable;
}
std::shared_ptr<Chunk> FlexibleWorld::getCurrentChunk() {
	return this->currentChunk;
}
// World
void FlexibleWorld::setViewPosition(const glm::vec3 & viewPosition) {
	this->viewPosition = viewPosition;
	Chunk::Instance newChunk = nullptr;
	for (auto fc : chunkVec) {
		newChunk = fc->chunk->lookup(this->viewPosition);
		if (newChunk) {
			break;
		}
	}
	if (!newChunk) {
		newChunk = loadChunk(viewPosition.x, viewPosition.z);
	}
	if (this->currentChunk != newChunk) {
		this->currentChunk = newChunk;
		this->updateNeighborChunks();
		for (auto fc : chunkVec) fc->chunk->tidyResource();
	}
}
void FlexibleWorld::setBiome(std::shared_ptr<Biome> biome) {
	this->biome = biome;
}
std::shared_ptr<Biome> FlexibleWorld::getBiome() const {
	return this->biome;
}
FlexibleChunkOffset FlexibleWorld::computeChunkOffset(int x, int z) const {
	int xOffset = (x / chunkXSize) * chunkXSize;
	if (x < 0) {
		int a = ((x % chunkXSize) / chunkXSize) - 1;
		xOffset += (a * chunkXSize);
	}
	int zOffset = (z / chunkZSize) * chunkZSize;
	if (z < 0) {
		int a = ((z % chunkZSize) / chunkZSize) - 1;
		zOffset += (a * chunkZSize);
	}
	return FlexibleChunkOffset(xOffset, zOffset);
}
std::shared_ptr<Chunk> FlexibleWorld::loadChunk(int x, int z) {
	auto c = findChunk(x, z);
	if (c) {
		return c;
	}
	auto offset = computeChunkOffset(x, z);
	for (auto fc : chunkVec) {
		if (fc->chunk->getXOffset() == offset.x && fc->chunk->getZOffset() == offset.z) {
			return fc->chunk;
		}
	}
	auto fc = std::make_shared< detail::FlexibleChunk>(*this, offset.x, offset.z, chunkXSize, chunkZSize);
	chunkVec.emplace_back(fc);
	return fc->chunk;
}
void FlexibleWorld::draw() {
	for (auto fc : chunkVec) {
		fc->chunk->draw();
	}
}
// private
FlexibleWorld::FlexibleWorld(ofShader & shader, int worldYSize) 
 : worldYSize(worldYSize),
   chunkXSize(32),
   chunkZSize(32),
   viewRange(64),
   viewPosition(0,0,0),
   chunkAllocateScale(2),
   chunkVec(),
   shader(shader),
   lightTable(128,128,128),
   currentChunk(nullptr) {
}
int FlexibleWorld::sign(int v) {
	if (v >= 0) {
		return 1;
	}
	return -1;
}
void FlexibleWorld::updateNeighborChunks() {
	int visibleChunks = 0;
	for (auto fc : chunkVec)fc->chunk->hide();
	for (auto fc : chunkVec) {
		auto neighbor = fc->chunk->getNeighbor(this->currentChunk, this->viewRange);
		for (auto nc : neighbor) {
			nc->setVisible(true);
			visibleChunks++;
		}
	}
}
std::shared_ptr<Chunk> FlexibleWorld::findChunk(int x, int z) const {
	for (auto fc : chunkVec) {
		if (fc->chunk->isContains(x, 0, z)) {
			return fc->chunk;
		}
	}
	return nullptr;
}
}
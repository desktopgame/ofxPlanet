#include "FlexibleWorld.hpp"
#include "Chunk.hpp"
#include "biome/Biome.hpp"
#include "BlockTable.hpp"
#include "BlockPack.hpp"
#include "Block.hpp"
#include "Sector.hpp"
#include "LightTable.hpp"
#include <algorithm>
#include <iostream>
#include <ofEvents.h>

namespace ofxPlanet {
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
		auto chunk = fc->getChunk();
		if (chunk->isContains(x, y, z)) {
			x -= chunk->getXOffset();
			z -= chunk->getZOffset();
			return fc->getBlock(x, y, z);
		}
	}
	return nullptr;
}
bool FlexibleWorld::isFilled(int x, int y, int z) const {
	if (y < 0) {
		return false;
	}
	auto block = getBlock(x, y, z);
	if (block == nullptr) {
		return false;
	}
	return block->getShape() == BlockShape::Block;
}
int FlexibleWorld::getBrightness(int x, int y, int z) const {
	for (auto fc : this->chunkVec) {
		auto chunk = fc->getChunk();
		if (chunk->isContains(x, y, z)) {
			x -= chunk->getXOffset();
			z -= chunk->getZOffset();
			return fc->getLightTable().getLight(x, y, z);
		}
	}
	return 15;
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
std::shared_ptr<Chunk> FlexibleWorld::getCurrentChunk() {
	return this->currentChunk;
}
// World
void FlexibleWorld::setViewPosition(const glm::vec3 & viewPosition) {
	this->viewPosition = viewPosition;
	Chunk::Instance newChunk = nullptr;
	for (auto fc : chunkVec) {
		auto chunk = fc->getChunk();
		if (!fc->isGenerated()) {
			continue;
		}
		newChunk = chunk->lookup(this->viewPosition);
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
		for (auto fc : chunkVec) fc->getChunk()->tidy();
	}
}
void FlexibleWorld::setBiome(std::shared_ptr<Biome> biome) {
	this->biome = biome;
}
std::shared_ptr<Biome> FlexibleWorld::getBiome() const {
	return this->biome;
}
void FlexibleWorld::setLoadRange(int loadRange) {
	this->loadRange = loadRange;
}
int FlexibleWorld::getLoadRange() const {
	return this->loadRange;
}
std::shared_ptr<Chunk> FlexibleWorld::findChunk(int x, int z) const {
	auto fc = findChunkImpl(x, z);
	if (fc == nullptr) {
		return nullptr;
	}
	return fc->getChunk();
}
std::shared_ptr<Chunk> FlexibleWorld::loadChunk(int x, int z) {
	bool _;
	return loadChunk(x, z, _);
}
std::shared_ptr<Chunk> FlexibleWorld::loadChunk(int x, int z, bool & isCreatedNewChunk) {
	auto fc = loadChunkImpl(x, z, isCreatedNewChunk);
	chunkVec.emplace_back(fc);
	return fc->getChunk();
}
std::shared_ptr<Chunk> FlexibleWorld::loadOrGenChunk(int x, int z) {
	return loadOrGenChunkImpl(x, z, 0, 0);
}
void FlexibleWorld::draw() {
	for (auto fc : chunkVec) {
		fc->getChunk()->draw();
	}
}
// private
FlexibleWorld::FlexibleWorld(ofShader & shader, int worldYSize) 
 : worldYSize(worldYSize),
   chunkXSize(32),
   chunkZSize(32),
   viewRange(64),
   loadRange(96),
   viewPosition(0,0,0),
   chunkVec(),
   shader(shader),
   lightTable(128,128,128),
   currentChunk(nullptr),
   biome(nullptr) {
}
std::shared_ptr<Sector> FlexibleWorld::findChunkImpl(int x, int z) const {
	int offsetX = computeChunkOffsetX(x);
	int offsetZ = computeChunkOffsetZ(z);
	for (auto fc : chunkVec) {
		auto chunk = fc->getChunk();
		if (chunk->isContains(x, 0, z) ||
			(chunk->getXOffset() == offsetX && chunk->getZOffset() == offsetZ)) {
			return fc;
		}
	}
	return nullptr;
}
std::shared_ptr<Sector> FlexibleWorld::loadChunkImpl(int x, int z, bool & isCreatedNewChunk) {
	isCreatedNewChunk = false;
	auto fc = findChunkImpl(x, z);
	if (fc) {
		return fc;
	}
	isCreatedNewChunk = true;
	int offsetX = computeChunkOffsetX(x);
	int offsetZ = computeChunkOffsetZ(z);
	fc = std::make_shared< Sector>(*this, offsetX, offsetZ, chunkXSize, chunkZSize);
	this->chunkVec.emplace_back(fc);
	return fc;
}
std::shared_ptr<Chunk> FlexibleWorld::loadOrGenChunkImpl(int x, int z, int xOffset, int zOffset) {
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
std::shared_ptr<Chunk> FlexibleWorld::loadOrGenChunkRange(int x, int z, int xOffset, int zOffset) {
	bool genCenter, genLeft, genRight, genTop, genBottom, genLTop, genRTop, genLBottom, genRBottom;
	auto centerFc = loadChunkImpl(x, z, genCenter);
	if (centerFc->isGenerated()) {
		return centerFc->getChunk();
	}
	int dx = computeGridX(x);
	int dz = computeGridZ(z);
	int cx = 2;
	int cz = 2;
	if (!(dx == cx && dz == cz)) {
		return nullptr;
	}

	auto leftFc = loadChunkImpl(x - chunkXSize, z, genLeft);
	auto leftTopFc = loadChunkImpl(x - chunkXSize, z - chunkZSize, genLTop);
	auto leftBottomFc = loadChunkImpl(x - chunkXSize, z + chunkZSize, genLBottom);

	auto rightFc = loadChunkImpl(x + chunkXSize, z, genRight);
	auto rightTopFc = loadChunkImpl(x + chunkXSize, z - chunkZSize, genRTop);
	auto rightBottomFc = loadChunkImpl(x + chunkXSize, z + chunkZSize, genRBottom);

	auto topFc = loadChunkImpl(x, z - chunkZSize, genTop);
	auto bottomFc = loadChunkImpl(x, z + chunkZSize, genBottom);
	auto bp = BlockPack::getCurrent();
	if (!centerFc->isGenerated() &&
		!leftFc->isGenerated() &&
		!rightFc->isGenerated() &&
		!topFc->isGenerated() &&
		!bottomFc->isGenerated() &&
		!leftTopFc->isGenerated() &&
		!rightTopFc->isGenerated() &&
		!leftBottomFc->isGenerated() &&
		!rightBottomFc->isGenerated()) {
		BlockTable bt(chunkXSize * 3, this->worldYSize, this->chunkZSize * 3);
		biome->generate(bt);
		for (int x = 0; x < chunkXSize; x++) {
			for (int z = 0; z < chunkZSize; z++) {
				for (int y = 0; y < this->worldYSize; y++) {
					leftTopFc->setBlock(x, y, z, bp->getBlock(bt.getBlock(x, y, z).id));
					topFc->setBlock(x, y, z, bp->getBlock(bt.getBlock((chunkXSize)+x, y, z).id));
					rightTopFc->setBlock(x, y, z, bp->getBlock(bt.getBlock((chunkXSize * 2) + x, y, z).id));
					//--
					leftFc->setBlock(x, y, z, bp->getBlock(bt.getBlock(x, y, z + chunkZSize).id));
					centerFc->setBlock(x, y, z, bp->getBlock(bt.getBlock((chunkXSize)+x, y, z + chunkZSize).id));
					rightFc->setBlock(x, y, z, bp->getBlock(bt.getBlock((chunkXSize * 2) + x, y, z + chunkZSize).id));
					//--
					leftBottomFc->setBlock(x, y, z, bp->getBlock(bt.getBlock(x, y, z + (chunkZSize * 2)).id));
					bottomFc->setBlock(x, y, z, bp->getBlock(bt.getBlock((chunkXSize)+x, y, z + (chunkZSize * 2)).id));
					rightBottomFc->setBlock(x, y, z, bp->getBlock(bt.getBlock((chunkXSize * 2) + x, y, z + (chunkZSize * 2)).id));
				}
			}
		}
		leftTopFc->setGenerated(true);
		topFc->setGenerated(true);
		rightTopFc->setGenerated(true);

		leftFc->setGenerated(true);
		centerFc->setGenerated(true);
		rightFc->setGenerated(true);

		leftBottomFc->setGenerated(true);
		bottomFc->setGenerated(true);
		rightBottomFc->setGenerated(true);

		return centerFc->getChunk();
	}
	return nullptr;
}
int FlexibleWorld::computeChunkOffsetX(int x) const {
	int xOffset = (x / chunkXSize) * chunkXSize;
	if (x < 0) {
		int a = ((x % chunkXSize) / chunkXSize) - 1;
		xOffset += (a * chunkXSize);
	}
	return xOffset;
}
int FlexibleWorld::computeChunkOffsetZ(int z) const {
	int zOffset = (z / chunkZSize) * chunkZSize;
	if (z < 0) {
		int a = ((z % chunkZSize) / chunkZSize) - 1;
		zOffset += (a * chunkZSize);
	}
	return zOffset;
}
void FlexibleWorld::updateNeighborChunks() {
	int visibleChunks = 0;
	for (auto fc : chunkVec)fc->getChunk()->hide();
	if (this->currentChunk == nullptr) {
		return;
	}
	for (auto fc : chunkVec) {
		auto chunk = fc->getChunk();
		auto neighbor = chunk->getNeighbor(this->currentChunk, this->viewRange);
		for (auto nc : neighbor) {
			nc->setVisible(true);
			visibleChunks++;
		}
	}
}
int FlexibleWorld::computeGridX(int x) const {
	int ox = x;
	int dx = 0;
	if (x >= 0) {
		dx = ((ox / this->chunkXSize) % 3);
		dx++;
	}
	else {
		dx = 4 - computeGridX(-x);
	}
	return dx;
}
int FlexibleWorld::computeGridZ(int z) const {
	int oz = z;
	int dz = 0;

	if (z >= 0) {
		dz = ((oz / this->chunkZSize) % 3);
		dz++;
	}
	else {
		dz = 4 - computeGridZ(-z);

	}
	return dz;
}
}
#include "FlexibleWorld.hpp"
#include "Chunk.hpp"
#include "biome/Biome.hpp"
#include "BlockTable.hpp"
#include "BlockPack.hpp"
#include "Block.hpp"
#include <algorithm>
#include <iostream>
#include <ofEvents.h>

namespace ofxPlanet {
// FlexibleChunk
namespace detail {
FlexibleChunk::FlexibleChunk(IWorld & world, int xOffset, int zOffset, int xSize, int zSize)
 : table(), 
   chunk(Chunk::create(world, xOffset, zOffset, xSize, zSize)),
   generated(false) {
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
	if (y < 0) {
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
		if (!fc->generated) {
			continue;
		}
		newChunk = fc->chunk->lookup(this->viewPosition);
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
std::shared_ptr<Chunk> FlexibleWorld::findChunk(int x, int z) const {
	auto fc = findChunkImpl(x, z);
	if (fc == nullptr) {
		return nullptr;
	}
	return fc->chunk;
}
std::shared_ptr<Chunk> FlexibleWorld::loadChunk(int x, int z) {
	bool _;
	return loadChunk(x, z, _);
}
std::shared_ptr<Chunk> FlexibleWorld::loadChunk(int x, int z, bool & isCreatedNewChunk) {
	auto fc = loadChunkImpl(x, z, isCreatedNewChunk);
	chunkVec.emplace_back(fc);
	return fc->chunk;
}
std::shared_ptr<Chunk> FlexibleWorld::loadOrGenChunk(int x, int z) {
	return loadOrGenChunkImpl(x, z, 0, 0);
}
void FlexibleWorld::draw() {
	bool k = ofGetKeyPressed(ofKey::OF_KEY_F11);
	for (auto fc : chunkVec) {
		fc->chunk->draw();
	}
	if (!k)return;
	for (auto ch : this->chunkVec) {

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
std::shared_ptr<detail::FlexibleChunk> FlexibleWorld::findChunkImpl(int x, int z) const {
	auto offset = computeChunkOffset(x, z);
	for (auto fc : chunkVec) {
		if (fc->chunk->isContains(x, 0, z) || 
			(fc->chunk->getXOffset() == offset.x && fc->chunk->getZOffset() == offset.z)) {
			return fc;
		}
	}
	return nullptr;
}
std::shared_ptr<detail::FlexibleChunk> FlexibleWorld::loadChunkImpl(int x, int z, bool & isCreatedNewChunk) {
	isCreatedNewChunk = false;
	auto fc = findChunkImpl(x, z);
	if (fc) {
		return fc;
	}
	isCreatedNewChunk = true;
	auto offset = computeChunkOffset(x, z);
	//std::cout << "load: x=" << x << " z=" << z << " ox=" << offset.x << " oz=" << offset.z << std::endl;
	fc = std::make_shared< detail::FlexibleChunk>(*this, offset.x, offset.z, chunkXSize, chunkZSize);
	this->chunkVec.emplace_back(fc);
	return fc;
}
std::shared_ptr<Chunk> FlexibleWorld::loadOrGenChunkImpl(int x, int z, int xOffset, int zOffset) {
	auto chk = loadOrGenChunkRange(x, z, xOffset, zOffset);
	if (chk != nullptr) {
		return chk;
	}
	int offsetX = 96;
	int offsetZ = 96;
	for (int addX = offsetX; addX > 0; addX--) {
		int ax = addX;
		for (int addZ = offsetZ; addZ > 0; addZ--) {
			int az = addZ;
			loadOrGenChunkRange(x + ax, z - az, 0, 0);
			loadOrGenChunkRange(x - ax, z + az, 0, 0);
			loadOrGenChunkRange(x + ax, z + az, 0, 0);
			loadOrGenChunkRange(x - ax, z - az, 0, 0);
		}
		//int az = addX;
		//loadOrGenChunkRange(x + ax, z - az, 0, 0);
		//loadOrGenChunkRange(x - ax, z + az, 0, 0);
		//loadOrGenChunkRange(x + ax, z + az, 0, 0);
		//loadOrGenChunkRange(x - ax, z - az, 0, 0);
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
	if (centerFc->generated) {
		//std::cout << "cached" << std::endl;
		return centerFc->chunk;
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
	if (!centerFc->generated &&
		!leftFc->generated &&
		!rightFc->generated &&
		!topFc->generated &&
		!bottomFc->generated &&
		!leftTopFc->generated &&
		!rightTopFc->generated &&
		!leftBottomFc->generated &&
		!rightBottomFc->generated) {
		BlockTable bt(chunkXSize * 3, this->worldYSize, this->chunkZSize * 3);
		biome->generate(bt);
		for (int x = 0; x < chunkXSize; x++) {
			for (int z = 0; z < chunkZSize; z++) {
				for (int y = 0; y < this->worldYSize; y++) {
					leftTopFc->table[x][y][z] = bp->getBlock(bt.getBlock(x, y, z).id);
					topFc->table[x][y][z] = bp->getBlock(bt.getBlock((chunkXSize)+x, y, z).id);
					rightTopFc->table[x][y][z] = bp->getBlock(bt.getBlock((chunkXSize * 2) + x, y, z).id);
					//--
					leftFc->table[x][y][z] = bp->getBlock(bt.getBlock(x, y, z + chunkZSize).id);
					centerFc->table[x][y][z] = bp->getBlock(bt.getBlock((chunkXSize)+x, y, z + chunkZSize).id);
					rightFc->table[x][y][z] = bp->getBlock(bt.getBlock((chunkXSize * 2) + x, y, z + chunkZSize).id);
					//--
					leftBottomFc->table[x][y][z] = bp->getBlock(bt.getBlock(x, y, z + (chunkZSize * 2)).id);
					bottomFc->table[x][y][z] = bp->getBlock(bt.getBlock((chunkXSize)+x, y, z + (chunkZSize * 2)).id);
					rightBottomFc->table[x][y][z] = bp->getBlock(bt.getBlock((chunkXSize * 2) + x, y, z + (chunkZSize * 2)).id);
				}
			}
		}
		leftTopFc->generated = true;
		topFc->generated = true;
		rightTopFc->generated = true;

		leftFc->generated = true;
		centerFc->generated = true;
		rightFc->generated = true;

		leftBottomFc->generated = true;
		bottomFc->generated = true;
		rightBottomFc->generated = true;

		std::cout << "loaded: x=" << x << " z=" << z << std::endl;
		return centerFc->chunk;
	}
	return nullptr;
}
void FlexibleWorld::updateNeighborChunks() {
	int visibleChunks = 0;
	for (auto fc : chunkVec)fc->chunk->hide();
	if (this->currentChunk == nullptr) {
		return;
	}
	for (auto fc : chunkVec) {
		auto neighbor = fc->chunk->getNeighbor(this->currentChunk, this->viewRange);
		for (auto nc : neighbor) {
			nc->setVisible(true);
			visibleChunks++;
			std::cout << "chunk: " << nc->getXOffset() << ", " << nc->getZOffset() << std::endl;
		}
	}
	std::cout << "--- " << visibleChunks << " ---" << std::endl;
}
int FlexibleWorld::computeGridX(int x) const {
	int ox = x;
	int dx = 0;
	if (x >= 0) {
		dx = ((ox / 32) % 3);
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
		dz = ((oz / 32) % 3);
		dz++;
	}
	else {
		dz = 4 - computeGridZ(-z);

	}
	return dz;
}
}
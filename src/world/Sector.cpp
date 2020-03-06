#include "Sector.hpp"
#include "IWorld.hpp"
#include "Chunk.hpp"
#include "Block.hpp"

namespace ofxPlanet {
Sector::Sector(IWorld & world, int xOffset, int zOffset, int xSize, int zSize)
 :  world(world),
	table(),
	chunk(Chunk::create(world, xOffset, zOffset, xSize, zSize)),
	lightTable(xSize, world.getYSize(), zSize),
	generated(false),
	invalidBrightCache(true) {
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
void Sector::setBlock(int x, int y, int z, std::shared_ptr<Block> block) {
	table[x][y][z] = block;
	this->invalidBrightCache = true;
}
std::shared_ptr<Block> Sector::getBlock(int x, int y, int z) const {
	return table[x][y][z];
}
bool Sector::isFilled(int x, int y, int z) const {
	int xo = chunk->getXOffset();
	int zo = chunk->getZOffset();
	bool overNegative = x < 0 || y < 0 || z < 0;
	bool overPositive = x >= chunk->getXSize() || y >= world.getYSize() || z >= chunk->getZSize();
	if (overNegative || overPositive) {
		auto block = world.getBlock(x, y, z);
		if (block == nullptr) {
			return false;
		}
		return block->getShape() == BlockShape::Block;
	}
	auto block = getBlock(x, y, z);
	if (block == nullptr) {
		return false;
	}
	return block->getShape() == BlockShape::Block;
}
std::shared_ptr<Chunk> Sector::getChunk() const {
	return this->chunk;
}
LightTable & Sector::getLightTable() {
	return this->lightTable;
}
const LightTable & Sector::getLightTable() const {
	return this->lightTable;
}
int Sector::getTopYForXZ(int x, int z) const {
	for (int y = world.getYSize() - 1; y >= 0; y--) {
		auto block = getBlock(x, y, z);
		if (block) {
			return y;
		}
	}
	return 0;
}
void Sector::computeBrightness() {
	if (!this->invalidBrightCache) {
		return;
	}
	for (int x = 0; x < this->chunk->getXSize(); x++) {
		for (int y = 0; y < this->world.getYSize(); y++) {
			for (int z = 0; z < this->chunk->getZSize(); z++) {
				auto block = getBlock(x, y, z);
				lightTable.setLight(x, y, z, 0);
			}
		}
	}
	for (int x = 0; x < this->chunk->getXSize(); x++) {
		for (int z = 0; z < this->chunk->getZSize(); z++) {
			int y = getTopYForXZ(x, z);
			int sunpower = 15;
			lightTable.setLight(x, y, z, sunpower--);
			for (; y >= 0 && sunpower > 0; y--) {
				auto block = getBlock(x, y, z);
				if (block != nullptr) {
					lightTable.setLight(x, y, z, sunpower--);
				}
			}
		}
	}
	this->invalidBrightCache = false;
}

void Sector::invalidateBrightness() {
	this->invalidBrightCache = true;
}
void Sector::setGenerated(bool b) {
	this->generated = b;
}
bool Sector::isGenerated() const {
	return this->generated;
}
}
#include "Sector.hpp"
#include "IWorld.hpp"
#include "Chunk.hpp"

namespace ofxPlanet {
Sector::Sector(IWorld & world, int xOffset, int zOffset, int xSize, int zSize)
 : table(),
	chunk(Chunk::create(world, xOffset, zOffset, xSize, zSize)),
	lightTable(xSize, world.getYSize(), zSize),
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
void Sector::setBlock(int x, int y, int z, std::shared_ptr<Block> block) {
	table[x][y][z] = block;
}
std::shared_ptr<Block> Sector::getBlock(int x, int y, int z) const {
	return table[x][y][z];
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
void Sector::setGenerated(bool b) {
	this->generated = b;
}
bool Sector::isGenerated() const {
	return this->generated;
}
}
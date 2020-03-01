#include "Sector.hpp"
namespace ofxPlanet {
Sector::Sector(int xOffset, int zOffset, int xChunkCount, int zChunkCount)
 : xOffset(xOffset),
   zOffset(zOffset),
   xChunkCount(xChunkCount),
   zChunkCount(zChunkCount),
   table() {
	for (int x = 0; x < xChunkCount; x++) {
		std::vector<std::shared_ptr<Chunk> > line;
		for (int z = 0; z < zChunkCount; z++) {
			line.emplace_back(nullptr);
		}
		table.emplace_back(line);
	}
}
int Sector::getXOffset() const {
	return xOffset;
}
int Sector::getZOffset() const {
	return zOffset;
}
int Sector::getXChunkCount() const {
	return xChunkCount;
}
int Sector::getZChunkCount() const {
	return zChunkCount;
}
void Sector::setChunk(int x, int z, std::shared_ptr<Chunk> chunk) {
	this->table[x][z] = chunk;
}
std::shared_ptr<Chunk> Sector::getChunk(int x, int z) const {
	return this->table[x][z];
}
}
#pragma once
#ifndef WORLD_SECTOR_HPP
#define WORLD_SECTOR_HPP
#include <vector>
#include <memory>

namespace ofxPlanet {
class Chunk;
class Sector {
public:
	explicit Sector(int xOffset, int zOffset, int xChunkCount, int zChunkCount);

	int getXOffset() const;
	int getZOffset() const;
	int getXChunkCount() const;
	int getZChunkCount() const;

	void setChunk(int x, int z, std::shared_ptr<Chunk> chunk);
	std::shared_ptr<Chunk> getChunk(int x, int z) const;
private:
	int xOffset;
	int zOffset;
	int xChunkCount;
	int zChunkCount;
	std::vector<
		std::vector<
			std::shared_ptr<Chunk>
		>
	> table;
};
}
#endif
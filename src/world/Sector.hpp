#pragma once
#ifndef WORLD_SECTOR_HPP
#define WORLD_SECTOR_HPP
#include <vector>
#include <memory>

namespace ofxPlanet {
class IWorld;
class Block;
class Chunk;
/**
 * Sector.
 */
class Sector {
public:
	explicit Sector(IWorld& world, int xOffset, int zOffset, int xSize, int zSize);

	void setBlock(int x, int y, int z, std::shared_ptr<Block> block);
	std::shared_ptr<Block> getBlock(int x, int y, int z) const;
	std::shared_ptr<Chunk> getChunk() const;

	void setGenerated(bool b);
	bool isGenerated() const;
private:
	std::vector<
		std::vector<
			std::vector<
				std::shared_ptr<Block>
			>
		>
	> table;
	std::shared_ptr<Chunk> chunk;
	bool generated;
};
}
#endif
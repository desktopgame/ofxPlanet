#ifndef WORLD_FLEXIBLEWORLD_HPP
#define WORLD_FLEXIBLEWORLD_HPP
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <ofShader.h>
#include "IWorld.hpp"

namespace ofxPlanet {
class Chunk;
class Block;
class Biome;

namespace detail {
/**
 * FlexibleChunk.
 */
class FlexibleChunk {
public:
	explicit FlexibleChunk(IWorld& world, int xOffset, int zOffset, int xSize, int zSize);

	std::vector<
		std::vector<
			std::vector<
				std::shared_ptr<Block>
			>
		>
	> table;
	std::shared_ptr<Chunk> chunk;
};
}

/**
 * FlexibleChunkOffset.
 */
struct FlexibleChunkOffset {
	explicit FlexibleChunkOffset(int x, int z);
	explicit FlexibleChunkOffset();
	int x;
	int z;
};
	
/**
 * FlexibleWorld is extensible in dynamic.
 * use this class, if want world generate system like minecraft.
 */
class FlexibleWorld : public IWorld {
public:
	using Instance = std::shared_ptr<FlexibleWorld>;
	using Reference = std::weak_ptr<FlexibleWorld>;
	static Instance create(ofShader& shader, int worldYSize);
	// IWorld
	void computeBrightness();
	int getYSize() const;
	std::shared_ptr<Block> getBlock(int x, int y, int z) const;
	bool isFilled(int x, int y, int z) const;
	ofShader& getShader();
	glm::vec3 getViewPosition() const;
	int getViewRange() const;
	ChunkLoadStyle getChunkLoadStyle() const;
	LightTable& getLightTable();
	std::shared_ptr<Chunk> getCurrentChunk();

	// FlexibleWorld
	void setViewPosition(const glm::vec3& viewPosition);
	void setBiome(std::shared_ptr<Biome> biome);
	std::shared_ptr<Biome> getBiome() const;
	FlexibleChunkOffset computeChunkOffset(int x, int z) const;
	std::shared_ptr<Chunk> findChunk(int x, int z) const;
	std::shared_ptr<Chunk> loadChunk(int x, int z);
	std::shared_ptr<Chunk> loadChunk(int x, int z, bool& isCreatedNewChunk);
	void draw();

private:
	FlexibleWorld(ofShader& shader, int worldYSize);
	static int sign(int v);
	void updateNeighborChunks();

	int worldYSize;
	int chunkXSize;
	int chunkZSize;
	int chunkAllocateScale;
	int viewRange;
	glm::vec3 viewPosition;
	std::vector<std::shared_ptr<detail::FlexibleChunk> > chunkVec;
	ofShader& shader;
	LightTable lightTable;
	std::shared_ptr<Chunk> currentChunk;
	std::shared_ptr<Biome> biome;
};
}
#endif
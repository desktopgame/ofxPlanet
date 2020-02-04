#pragma once
#ifndef WORLD_CHUNK_HPP
#define WORLD_CHUNK_HPP
#include "BlockRenderer.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace ofxPlanet {
class World;
/**
 * ChunkType.
 */
enum class ChunkType {
	Single,
	Split,
};
/**
 * Chunk is part of world.
 */
class Chunk : public std::enable_shared_from_this<Chunk> {
public:
	using Instance = std::shared_ptr<Chunk>;
	using Reference = std::weak_ptr<Chunk>;
	~Chunk();
	/**
	 * returns new chunk.
	 * @param world
	 * @param xOffset
	 * @param yOffset
	 * @param xSize
	 * @param zSize
	 * @return
	 */
	static Instance create(World& world, int xOffset, int zOffset, int xSize, int zSize);
	/**
	 * this chunk mark as `taint`.
	 */
	void invalidate();
	/**
	 * this chunk mark as `taint`.
	 * @param x
	 * @param y
	 * @param z
	 */
	void invalidate(int x, int y, int z);
	/**
	 * recompute vertex information, if marked as `taint`.
	 */
	void rehash();
	/**
	 * draw chunk.
	 */
	void draw();

	/**
	 * return true if included in chunk for specific position.
	 * @param x
	 * @param y
	 * @param z
	 * @return
	 */
	bool isContains(int x, int y, int z) const;
	/**
	 * return true if marked as `taint`.
	 * @return
	 */
	bool isInvalid() const;
	/**
	 * returns world.
	 * @return
	 */
	const World& getWorld() const;

	/**
	 * split chunks.
	 * @param splitSize
	 */
	void split(int splitSize);

	/**
	 * returns type.
	 * @return
	 */
	ChunkType getType() const;

	/**
	 * returns parent.
	 * @return
	 */
	Instance getParent() const;
private:
	explicit Chunk(Reference parent, World& world, int xOffset, int zOffset, int xSize, int zSize);
	void deleteRenderer();

	ChunkType type;
	bool invalid;
	World& world;
	BlockRenderer* renderer;
	int xOffset;
	int zOffset;
	int xSize;
	int zSize;
	Reference parent;
	std::vector<Instance> subchunks;
};
}
#endif
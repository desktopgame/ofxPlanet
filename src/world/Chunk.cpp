#include "Chunk.hpp"
#include "World.hpp"
#include "Block.hpp"
#include "Chunk.hpp"

namespace ofxPlanet {
Chunk::~Chunk() {
	deleteRenderer();
}
Chunk::Instance Chunk::create(World & world, int xOffset, int zOffset, int xSize, int zSize) {
	return Instance(new Chunk(Reference(), world, xOffset, zOffset, xSize, zSize));
}
void Chunk::invalidate() {
	if (invalid) {
		return;
	}
	if (this->type == ChunkType::Split) {
		for (auto subchunk : subchunks) {
			subchunk->invalidate();
		}
	}
	this->invalid = true;
}

void Chunk::invalidate(int x, int y, int z) {
	if (!isContains(x, y, z) || invalid) {
		return;
	}
	if (this->type == ChunkType::Split) {
		for (auto subchunk : subchunks) {
			subchunk->invalidate(x, y, z);
		}
	}
	this->invalid = true;
}
void Chunk::rehash() {
	if (!this->invalid) {
		return;
	}
	if (this->type == ChunkType::Single) {
		renderer->clear();
		for (int x = xOffset; x < xOffset + xSize; x++) {
			for (int z = zOffset; z < zOffset + zSize; z++) {
				for (int y = 0; y < world.getYSize(); y++) {
					auto block = world.getBlock(x, y, z);
					if (block != nullptr) {
						block->batch(this->world, *renderer, x, y, z);
					}
				}
			}
		}
		renderer->update();
	} else {
		for (auto subchunk : subchunks) {
			subchunk->rehash();
		}
	}
	this->invalid = false;
}
void Chunk::draw() {
	rehash();
	if (this->type == ChunkType::Single) {
		renderer->render();
	} else {
		for (auto subchunk : subchunks) {
			subchunk->draw();
		}
	}
}
bool Chunk::isContains(int x, int y, int z) const {
	if (x < xOffset || y < 0 || z < zOffset) {
		return false;
	}
	if (x >= (xOffset + xSize) || y >= world.getYSize() || z >= (zOffset + zSize)) {
		return false;
	}
	return true;
}
bool Chunk::isInvalid() const {
	return invalid;
}
const World & Chunk::getWorld() const {
	return world;
}
void Chunk::split(int splitSize) {
	if (this->type == ChunkType::Split) {
		throw std::logic_error("this chunks is already split");
	}
	int xSplit = xSize / splitSize;
	int zSplit = zSize / splitSize;
	int xSplitM = xSize % splitSize;
	int zSplitM = zSize % splitSize;
	auto self = std::const_pointer_cast<Chunk>(shared_from_this());
	for (int x = 0; x < xSize; x += xSplit) {
		for (int z = 0; z < zSize; z += zSplit) {
			auto subChunk = Instance(new Chunk(self, world, x, z, xSplit, zSplit));
			subchunks.emplace_back(subChunk);
		}
	}
	if (xSplitM > 0 || zSplitM > 0) {
		throw std::logic_error("invalid split size");
	}
	deleteRenderer();
	this->type = ChunkType::Split;
}
ChunkType Chunk::getType() const {
	return this->type;
}
Chunk::Instance Chunk::getParent() const {
	return parent.lock();
}
// private
Chunk::Chunk(Reference parent, World& world, int xOffset, int zOffset, int xSize, int zSize)
	:
	type(ChunkType::Single),
	invalid(true),
	world(world),
	renderer(new BlockRenderer(world, world.getShader())),
	xOffset(xOffset),
	zOffset(zOffset),
	xSize(xSize),
	zSize(zSize),
	parent(parent),
	subchunks() {
}
void Chunk::deleteRenderer() {
	if (this->renderer != nullptr) {
		delete renderer;
		this->renderer = nullptr;
	}
}
}

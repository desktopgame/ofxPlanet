#include "Chunk.hpp"

#include <ofGraphics.h>

#include "Block.hpp"
#include "Chunk.hpp"
#include "World.hpp"

namespace ofxPlanet {
Chunk::~Chunk() { deleteRenderer(); }
Chunk::Instance Chunk::create(IWorld& world, int xOffset, int zOffset, int xSize,
                              int zSize) {
        return Instance(
            new Chunk(Reference(), world, xOffset, zOffset, xSize, zSize));
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
        if (!isContains(x, y, z)) {
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
        this->world.computeBrightness();
        ChunkLoadStyle style = world.getChunkLoadStyle();
        if (style == ChunkLoadStyle::VisibleChunk) {
                rehashVisible();
        } else {
                rehashAll();
        }
        this->invalid = false;
}
void Chunk::draw() {
        rehash();
        if (this->type == ChunkType::Single) {
                if (this->renderer != nullptr) {
                        renderer->render();
                }
        } else {
                for (auto subchunk : subchunks) {
                        subchunk->draw();
                }
        }
}
void Chunk::debugDraw() {
        float scale = 2.0f;
        glm::vec3 vscale = glm::vec3(1, scale, 1);
        ofDrawLine(glm::vec3(xOffset, 0, zOffset) * vscale,
                   glm::vec3(xOffset, world.getYSize(), zOffset) * vscale);
        ofDrawLine(
            glm::vec3(xOffset + xSize, 0, zOffset) * vscale,
            glm::vec3(xOffset + xSize, world.getYSize(), zOffset) * vscale);
        ofDrawLine(
            glm::vec3(xOffset, 0, zOffset + zSize) * vscale,
            glm::vec3(xOffset, world.getYSize(), zOffset + zSize) * vscale);
        ofDrawLine(
            glm::vec3(xOffset + xSize, 0, zOffset + zSize) * vscale,
            glm::vec3(xOffset + xSize, world.getYSize(), zOffset + zSize) *
                vscale);

        for (auto subchunk : subchunks) {
                subchunk->draw();
        }
}
bool Chunk::isContains(int x, int y, int z) const {
        if (x < xOffset || y < 0 || z < zOffset) {
                return false;
        }
        if (x >= (xOffset + xSize) || y >= world.getYSize() ||
            z >= (zOffset + zSize)) {
                return false;
        }
        return true;
}
bool ofxPlanet::Chunk::isContains(const glm::ivec3& pos) const {
        return isContains(pos.x, pos.y, pos.z);
}
bool Chunk::isInvalid() const { return invalid; }
const IWorld& Chunk::getWorld() const { return world; }
void Chunk::split(int splitSize) {
        if (this->type == ChunkType::Split) {
                throw std::logic_error("this chunks is already split");
        }
        auto self = std::const_pointer_cast<Chunk>(shared_from_this());
        for (int x = 0; x < xSize; x += splitSize) {
                for (int z = 0; z < zSize; z += splitSize) {
                        auto subChunk = Instance(
                            new Chunk(self, world, x, z, splitSize, splitSize));
                        subchunks.emplace_back(subChunk);
                }
        }
        deleteRenderer();
        this->type = ChunkType::Split;
}
ChunkType Chunk::getType() const { return this->type; }
Chunk::Instance Chunk::getParent() const { return parent.lock(); }
Chunk::Instance Chunk::lookup(int x, int y, int z) const {
        if (!isContains(x, y, z)) {
                return nullptr;
        }
        auto e = std::const_pointer_cast<Chunk>(this->shared_from_this());
        while (e) {
                if (e->subchunks.empty()) {
                        break;
                }
                for (auto subchunk : e->subchunks) {
                        auto c = subchunk->lookup(x, y, z);
                        if (c) {
                                e = c;
                                break;
                        }
                }
        }
        return e;
}
Chunk::Instance Chunk::lookup(const glm::ivec3& pos) const {
        return lookup(pos.x, pos.y, pos.z);
}
std::vector<Chunk::Instance> Chunk::getNeighbor(const Instance & chunk, int viewRange) const {
	std::vector<Chunk::Instance> v;
	int diffX = std::max(chunk->xOffset, this->xOffset) - std::min(chunk->xOffset, this->xOffset);
	int diffZ = std::max(chunk->zOffset, this->zOffset) - std::min(chunk->zOffset, this->zOffset);
	bool isIncludedViewRange = diffX < viewRange && diffZ < viewRange;
	if (isIncludedViewRange) {
		v.emplace_back(std::const_pointer_cast<Chunk>(shared_from_this()));
	}
	for (auto subchunk : subchunks) {
		auto cv = subchunk->getNeighbor(chunk, viewRange);
		for (auto ce : cv) {
			v.emplace_back(ce);
		}
	}
	return v;
}
Chunk::Instance Chunk::getSubChunk(int index) const {
	return subchunks.at(index);
}
int Chunk::getSubChunkCount() const {
	return static_cast<int>(subchunks.size());
}
int Chunk::getXOffset() const {
	return this->xOffset;
}
int Chunk::getZOffset() const {
	return this->zOffset;
}
int Chunk::getXSize() const {
	return this->xSize;
}
int Chunk::getZSize() const {
	return this->zSize;
}

void Chunk::setVisible(bool visible) {
	if (this->visible != visible) {
		if (visible) {
			allocateRenderer();
		} else {
			deleteRenderer();
		}
	}
	this->visible = visible;
}

bool Chunk::isVisible() const {
	return this->visible;
}

void Chunk::show() {
	Chunk::setVisibleRecursive(std::const_pointer_cast<Chunk>(shared_from_this()), true);
}

void Chunk::hide() {
	Chunk::setVisibleRecursive(std::const_pointer_cast<Chunk>(shared_from_this()), false);
}

// private
Chunk::Chunk(Reference parent, IWorld& world, int xOffset, int zOffset,
             int xSize, int zSize)
    : type(ChunkType::Single),
      invalid(true),
      world(world),
      renderer(new BlockRenderer(world.getShader())),
      xOffset(xOffset),
      zOffset(zOffset),
      xSize(xSize),
      zSize(zSize),
      parent(parent),
      subchunks() {}
void Chunk::setVisibleRecursive(Instance chunk, bool visible) {
	chunk->setVisible(visible);
	for (auto subchunk : chunk->subchunks) {
		setVisibleRecursive(subchunk, visible);
	}
}
void Chunk::allocateRenderer() {
        if (this->renderer == nullptr) {
                this->renderer = new BlockRenderer(world.getShader());
        }
}
void Chunk::deleteRenderer() {
        if (this->renderer != nullptr) {
                delete renderer;
                this->renderer = nullptr;
        }
}
void Chunk::batch() {
        renderer->clear();
        for (int x = xOffset; x < xOffset + xSize; x++) {
                for (int z = zOffset; z < zOffset + zSize; z++) {
                        for (int y = 0; y < world.getYSize(); y++) {
                                auto block = world.getBlock(x, y, z);
                                if (block != nullptr) {
                                        int brightness =
                                            world.getLightTable().getLight(x, y,
                                                                           z);
                                        block->batch(this->world, *renderer,
                                                     brightness, x, y, z);
                                }
                        }
                }
        }
        renderer->update();
}
void Chunk::rehashAll() {
        if (this->type == ChunkType::Single) {
                this->batch();
        } else {
                for (auto subchunk : subchunks) {
                        subchunk->rehash();
                }
        }
}
void Chunk::rehashVisible() {
        this->invalid = false;
        if (!isVisible()) {
                deleteRenderer();
                return;
        }
        if (this->type == ChunkType::Single) {
                this->allocateRenderer();
                this->batch();
        } else {
                for (auto subchunk : subchunks) {
                        subchunk->rehashVisible();
                }
        }
}
}  // namespace ofxPlanet

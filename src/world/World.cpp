#include "World.hpp"

#include <ofAppRunner.h>
#include <ofEasyCam.h>
#include <ofFbo.h>
#include <ofGraphics.h>
#include <ofLight.h>

#include <algorithm>
#include <string>

#include "Math.hpp"
#include "Chunk.hpp"
#include "Block.hpp"
#include "BlockPack.hpp"
#include "BlockRenderer.hpp"
#include "TexturePack.hpp"
namespace ofxPlanet {

WorldPart::WorldPart(const std::shared_ptr<World>& world, glm::ivec3 offset)
    : world(world), offset(offset) {}
std::shared_ptr<World> World::create(ofShader& shader, const glm::ivec3& size) {
        return create(shader, size.x, size.y, size.z);
}

std::shared_ptr<World> World::create(ofShader& shader, int xSize, int ySize,
                                     int zSize) {
        World* world = new World(shader, xSize, ySize, zSize);
        std::shared_ptr<World> ret = std::shared_ptr<World>(world);
        for (int i = 0; i < xSize; i++) {
                std::vector<std::vector<std::shared_ptr<Block> > > yline;
                for (int j = 0; j < ySize; j++) {
                        std::vector<std::shared_ptr<Block> > zline;
                        for (int k = 0; k < zSize; k++) {
                                zline.emplace_back(nullptr);
                        }
                        yline.emplace_back(zline);
                }
                ret->blocks.emplace_back(yline);
        }
        return ret;
}


void World::load(const BlockTable& table) {
        auto bp = BlockPack::getCurrent();
        for (int x = 0; x < table.getXSize(); x++) {
                for (int y = 0; y < table.getYSize(); y++) {
                        for (int z = 0; z < table.getZSize(); z++) {
                                BlockPrefab i = table.get(x, y, z);
                                if (i.id == -1) {
                                        continue;
                                }
                                if (i.instanced) {
                                        setBlock(x, y, z, bp->getBlock(i.id));
                                } else {
                                        setBlock(x, y, z, bp->getBlock(i.id));
                                }
                        }
                }
        }
}

void World::clear() {
        for (int x = 0; x < xSize; x++) {
                for (int y = 0; y < ySize; y++) {
                        for (int z = 0; z < zSize; z++) {
                                setBlock(x, y, z, nullptr);
                        }
                }
        }
		chunk->invalidate();
}

void World::update() {
}

void World::drawToBuffer() {
        checkFBO();
		chunk->rehash();
        fbo.begin();
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO,
                            GL_ONE);
        glEnable(GL_DEPTH_TEST);
        fbo.clear();
        ofEnableAlphaBlending();
        ofSetBackgroundColor(0, 0, 0, 0);
		chunk->draw();
        fbo.end();
}

void World::render() {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
}

RaycastResult World::raycast(glm::vec3 origin, glm::vec3 direction, float length, float scale) const {
	origin /= scale;
	int x = (Math::floatToInt(origin.x));
	int y = (Math::floatToInt(origin.y));
	int z = (Math::floatToInt(origin.z));
	float dx = direction.x;
	float dy = direction.y;
	float dz = direction.z;
	float stepX = Math::signum(dx);
	float stepY = Math::signum(dy);
	float stepZ = Math::signum(dz);
	float tMaxX = Math::intbound(origin.x, dx);
	float tMaxY = Math::intbound(origin.y, dy);
	float tMaxZ = Math::intbound(origin.z, dz);
	float tDeltaX = stepX / dx;
	float tDeltaY = stepY / dy;
	float tDeltaZ = stepZ / dz;
	int wx = (this->xSize);
	int wy = (this->ySize);
	int wz = (this->zSize);
	glm::vec3 normal(0, 0, 0);
	RaycastResult res;
	res.hit = false;
	if (Math::isZero(dx) && Math::isZero(dy) && Math::isZero(dz)) {
		return res;
	}
	length /= std::sqrt(dx*dx + dy * dy + dz * dz);

	while (/* ray has not gone past bounds of world */
		(stepX > 0 ? x < wx : x >= 0) &&
		(stepY > 0 ? y < wy : y >= 0) &&
		(stepZ > 0 ? z < wz : z >= 0)) {
		res.hit = false;
		// Invoke the callback, unless we are not *yet* within the bounds of the
		// world.
		if (!(x < 0 || y < 0 || z < 0 || x >= wx || y >= wy || z >= wz)) {
			res.position = glm::vec3(x, y, z);
			res.normal = normal;
			res.hit = true;
			if (isContains(res.position) && getBlock(res.position) != nullptr) {
				break;
			}
		}
		// tMaxX stores the t-value at which we cross a cube boundary along the
		// X axis, and similarly for Y and Z. Therefore, choosing the least tMax
		// chooses the closest cube boundary. Only the first case of the four
		// has been commented in detail.
		if (tMaxX < tMaxY) {
			if (tMaxX < tMaxZ) {
				if (tMaxX > length) break;
				// Update which cube we are now in.
				x += stepX;
				// Adjust tMaxX to the next X-oriented boundary crossing.
				tMaxX += tDeltaX;
				// Record the normal vector of the cube face we entered.
				normal.x = -stepX;
				normal.y = 0;
				normal.z = 0;
			} else {
				if (tMaxZ > length) break;
				z += stepZ;
				tMaxZ += tDeltaZ;
				normal.x = 0;
				normal.y = 0;
				normal.z = -stepZ;
			}
		} else {
			if (tMaxY < tMaxZ) {
				if (tMaxY > length) break;
				y += stepY;
				tMaxY += tDeltaY;
				normal.x = 0;
				normal.y = -stepY;
				normal.z = 0;
			} else {
				// Identical to the second case, repeated for simplicity in
				// the conditionals.
				if (tMaxZ > length) break;
				z += stepZ;
				tMaxZ += tDeltaZ;
				normal.x = 0;
				normal.y = 0;
				normal.z = -stepZ;
			}
		}
	}
	return res;
}

void World::setBlock(glm::vec3 pos, std::shared_ptr<Block> block) {
        setBlock(pos.x, pos.y, pos.z, block);
}

void World::setBlock(glm::ivec3 pos, std::shared_ptr<Block> block) {
        setBlock(pos.x, pos.y, pos.z, block);
}

void World::setBlock(float x, float y, float z, std::shared_ptr<Block> block) {
        setBlock(Math::floatToInt(x), Math::floatToInt(y),
			Math::floatToInt(z), block);
}

void World::setBlock(int x, int y, int z, std::shared_ptr<Block> block) {
        blocks[x][y][z] = block;
		chunk->invalidate(x+1, y, z);
		chunk->invalidate(x, y+1, z);
		chunk->invalidate(x, y, z+1);
		chunk->invalidate(x - 1, y, z);
		chunk->invalidate(x, y - 1, z);
		chunk->invalidate(x, y, z - 1);
		chunk->invalidate(x, y, z);
}

std::shared_ptr<Block> World::getBlock(int x, int y, int z) const {
        return blocks[x][y][z];
}
std::shared_ptr<Block> World::getBlock(float x, float y, float z) const {
        return getBlock(Math::floatToInt(x), Math::floatToInt(y),
			Math::floatToInt(z));
}
std::shared_ptr<Block> World::getBlock(glm::vec3 pos) const {
        return getBlock(pos.x, pos.y, pos.z);
}
std::shared_ptr<Block> World::getBlock(glm::ivec3 pos) const {
        return getBlock(pos.x, pos.y, pos.z);
}

bool World::isContains(int x, int y, int z) const {
        if (x < 0 || x >= xSize) return false;
        if (y < 0 || y >= ySize) return false;
        if (z < 0 || z >= zSize) return false;
        return true;
}
bool World::isContains(const glm::ivec3& v) const {
        return isContains(v.x, v.y, v.z);
}
bool World::isContains(const glm::vec3& v) const {
        return isContains(Math::floatToInt(v.x), Math::floatToInt(v.y), Math::floatToInt(v.z));
}
bool World::isEmpty(int x, int y, int z) const {
        if (!isContains(x, y, z)) {
                return true;
        }
        auto block = getBlock(x, y, z);
        return block == nullptr;
}
bool World::isFilled(int x, int y, int z) const {
        if (isEmpty(x, y, z)) {
                return false;
        }
        return getBlock(x, y, z)->getShape() == BlockShape::Block;
}
int World::getGroundY(int x, int z) const {
        for (int i = 0; i < ySize; i++) {
                if (!getBlock(x, i, z)) {
                        return i;
                }
        }
        return ySize;
}
glm::vec3 World::getPhysicalPosition(int x, int y, int z) const {
        glm::vec3 pos(x * 2, y * 2, z * 2);
        return pos;
}
int World::getXSize() const { return xSize; }
int World::getYSize() const { return ySize; }
int World::getZSize() const { return zSize; }
glm::ivec3 World::getSize() const { return glm::ivec3(xSize, ySize, zSize); }

std::vector<WorldPart> World::split(int splitNum) const {
        int sx = xSize / splitNum;
        int sz = zSize / splitNum;
        std::vector<WorldPart> ret;
        for (int i = 0; i < splitNum; i++) {
                for (int j = 0; j < splitNum; j++) {
                        auto w =
                            World::create(shader, glm::ivec3(sx, ySize, sz));
                        for (int x = (sx * i); x < (sx * i) + sx; x++) {
                                for (int y = 0; y < ySize; y++) {
                                        for (int z = (sz * j);
                                             z < (sz * j) + sz; z++) {
                                                int ix = x - (sx * i);
                                                int iz = z - (sz * j);
                                                w->setBlock(
                                                    glm::ivec3(ix, y, iz),
                                                    this->getBlock(x, y, z));
                                        }
                                }
                        }
                        float xoffs = (sx * splitNum * 2) - ((sx * i) * 2);
                        float zoffs = ((sz * j) * 2);
                        ret.emplace_back(
                            WorldPart(w, glm::ivec3(xoffs, 0, zoffs)));
                }
        }
        return ret;
}

std::shared_ptr<Chunk> World::getChunk() const {
	return chunk;
}

ofShader & World::getShader() {
	return shader;
}

void World::checkFBO() {
        int newW = ofGetWidth();
        int newH = ofGetHeight();
        if (this->fboW != newW || this->fboH != newH) {
                fbo.allocate(newW, newH);
                this->fboW = newW;
                this->fboH = newH;
        }
}

// private
World::World(ofShader& shader, const glm::ivec3& size)
    : World(shader, size.x, size.y, size.z) {}

World::World(ofShader& shader, int xSize, int ySize, int zSize)
    : blocks(),
      xSize(xSize),
      ySize(ySize),
      zSize(zSize),
      fbo(),
	  chunk(Chunk::create(*this, 0, 0, xSize, zSize)),
      shader(shader),
      fboW(-1),
      fboH(-1) {}
}  // namespace ofxPlanet

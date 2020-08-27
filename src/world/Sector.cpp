#include "Sector.hpp"

#include "Block.hpp"
#include "Chunk.hpp"
#include "IWorld.hpp"

namespace ofxPlanet {
Sector::Sector(IWorld& world, int xOffset, int zOffset, int xSize, int zSize)
    : world(world),
      table(),
      chunk(Chunk::create(world, xOffset, zOffset, xSize, zSize)),
      lightTable(xSize, world.getYSize(), zSize),
      generated(false),
      invalidBrightCache(true) {
        for (int x = 0; x < xSize; x++) {
                std::vector<std::vector<std::shared_ptr<Block> > > grid;
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
        bool overPositive = x >= chunk->getXSize() || y >= world.getYSize() ||
                            z >= chunk->getZSize();
        if (overNegative || overPositive) {
                auto block = world.getBlock(x + chunk->getXOffset(), y,
                                            z + chunk->getZOffset());
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
std::shared_ptr<Chunk> Sector::getChunk() const { return this->chunk; }
LightTable& Sector::getLightTable() { return this->lightTable; }
const LightTable& Sector::getLightTable() const { return this->lightTable; }
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
                                lightTable.set(x, y, z, 0);
                        }
                }
        }
        for (int x = 0; x < this->chunk->getXSize(); x++) {
                for (int z = 0; z < this->chunk->getZSize(); z++) {
                        int y = getTopYForXZ(x, z);
						int sunpower = world.getSunBrightness();
                        lightTable.set(x, y, z, sunpower--);
                        for (; y >= 0 && sunpower > 0; y--) {
                                auto block = getBlock(x, y, z);
                                if (block != nullptr) {
                                        lightTable.set(x, y, z,
                                                            sunpower--);
                                }
                        }
                }
        }
		for (int x = 0; x < this->chunk->getXSize(); x++) {
			for (int y = 0; y < this->world.getYSize(); y++) {
				for (int z = 0; z < this->chunk->getZSize(); z++) {
					auto block = getBlock(x, y, z);
					if (!block) { continue; }
					lightDiffusion(x, y, z, block->getBrightness());
				}
			}
		}
        this->invalidBrightCache = false;
}

void Sector::invalidateBrightness() { this->invalidBrightCache = true; }
void Sector::setGenerated(bool b) { this->generated = b; }
bool Sector::isGenerated() const { return this->generated; }
// private
void Sector::lightDiffusion(int x, int y, int z, int brightness) {
	int l = lightTable.get(x, y, z);
	lightTable.set(x, y, z, std::min(LightTable::BRIGHTNESS_MAX, l + brightness));

	struct Ring {
		glm::ivec3 a;
		glm::ivec3 b;
		glm::ivec3 c;
		glm::ivec3 d;
		glm::ivec3 e;
		glm::ivec3 f;
		glm::ivec3 g;
		glm::ivec3 h;
	};
	std::vector<Ring> rings;
	for (int i = 1; i < brightness; i++) {
		int xp = x + i;
		int xn = x - i;
		int yp = y + i;
		int yn = y - i;
		int zp = z + i;
		int zn = z - i;

		Ring ring;
		ring.a = glm::ivec3(xp, y, zp);
		ring.b = glm::ivec3(x, y, zp);
		ring.c = glm::ivec3(xn, y, zp);

		ring.d = glm::ivec3(xp, y, zn);
		ring.e = glm::ivec3(x, y, zn);
		ring.f = glm::ivec3(xn, y, zn);

		ring.g = glm::ivec3(xp, y, z);
		ring.h = glm::ivec3(xn, y, z);
		rings.emplace_back(ring);
	}
}
void Sector::lightDiffusion2(int x, int y, int z, BlockSide side) {
	if (!lightTable.contains(x, y, z)) {
		return;
	}
}
}  // namespace ofxPlanet
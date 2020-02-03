#include "Generator.hpp"

#include <iostream>
namespace planet {

Generator::Generator(const glm::ivec3& size) : size(size), sivPerlin() {}

Generator::Generator(int xSize, int ySize, int zSize)
    : Generator(glm::ivec3(xSize, ySize, zSize)) {}

Terrain Generator::generate(unsigned int seed) {
        sivPerlin.reseed(seed);
        Terrain ter;
        int xzSize = (size.x + size.z) / 2;
        // PerlinNoise noise((size.x + size.z), seed, freq);
        std::vector<Cell> cells;
        const float frequency = 5.0f;
        const float fx = size.x / frequency;
        const float fy = size.z / frequency;
        const float fz = size.y / frequency;
        for (int i = 0; i < size.x * size.z; i++) {
                int ix = i % xzSize;
                int iy = i / xzSize;
                float noise = static_cast<float>(
                    sivPerlin.octaveNoise(ix / fx, iy / fy, frequency));
                for (int z = 0; z < size.y; z++) {
                        if (z > this->size.y / 2) {
                                continue;
                        }
                        float thickP = static_cast<float>(sivPerlin.octaveNoise(
                            ix / fx, iy / fy, z / fz, frequency));
                        ter.addPocket(Pocket(ix, z, iy, thickP));
                }
                ter.addCell(Cell(ix, iy, noise));
        }
        return ter;
}
}  // namespace planet

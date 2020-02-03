#ifndef GAME_WORLD_GEN_GENERATOR_HPP
#define GAME_WORLD_GEN_GENERATOR_HPP
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "PerlinNoise/PerlinNoise.hpp"
#include "../World.hpp"
#include "Terrain.hpp"
namespace planet {
class Generator {
       public:
        explicit Generator(const glm::ivec3& size);
        explicit Generator(int xSize, int ySize, int zSize);
        Terrain generate(unsigned int seed);

        glm::ivec3 size;

       private:
        siv::PerlinNoise sivPerlin;
};
}  // namespace planet
#endif
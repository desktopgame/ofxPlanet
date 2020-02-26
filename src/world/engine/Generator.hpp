#ifndef GAME_WORLD_GEN_GENERATOR_HPP
#define GAME_WORLD_GEN_GENERATOR_HPP
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "../World.hpp"
#include "PerlinNoise/PerlinNoise.hpp"
#include "Terrain.hpp"
namespace ofxPlanet {
class Generator {
       public:
        explicit Generator(const glm::ivec3& size);
        explicit Generator(int xSize, int ySize, int zSize);
        Terrain generate(unsigned int seed);

       private:
        siv::PerlinNoise sivPerlin;
        glm::ivec3 size;
};
}  // namespace ofxPlanet
#endif
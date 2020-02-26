#pragma once
#ifndef WORLD_PLANET_HPP
#define WORLD_PLANET_HPP
#include <ofShader.h>

#include <glm/glm.hpp>
#include <memory>

#include "BlockTable.hpp"
namespace ofxPlanet {

class WorldGenerateService;
class World;
class FlexibleWorld;
class Biome;
class Camera;
/**
 * Planet is pair for World, and BlockTable.
 */
class Planet {
       public:
        explicit Planet(ofShader& shader);
        /**
         * generate world.
         * @param size
         * @param biome
         */
        void generate(glm::vec3 size, std::shared_ptr<Biome> biome);

        /**
         * @param size
         */
        void allocate(glm::ivec3 size);

        /**
         * update world.
         */
        void update();

        /**
         * returns world.
         * @return
         */
        std::shared_ptr<FlexibleWorld> getWorld() const;

        /**
         * returns blockTable.
         * @return
         */
        BlockTable getBlockTable() const;

       private:
        ofShader& shader;
        BlockTable blockTable;
        std::shared_ptr<FlexibleWorld> world;
};
}  // namespace ofxPlanet
#endif
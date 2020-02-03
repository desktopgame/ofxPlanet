#pragma once
#ifndef WORLD_BIOME_BIOME_HPP
#define WORLD_BIOME_BIOME_HPP

#include <memory>
namespace ofxPlanet {

class BlockTable;
class World;
/**
 * Biome is terrain generation algorithm in world.
 */
class Biome {
       public:
        Biome() = default;
        virtual ~Biome() = default;
        /**
         * write block data to block table.
         * @param blockTable
         */
        virtual void generate(BlockTable& blockTable) = 0;

       private:
};
}  // namespace ofxPlanet
#endif
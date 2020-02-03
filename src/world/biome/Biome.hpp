#pragma once
#ifndef WORLD_BIOME_BIOME_HPP
#define WORLD_BIOME_BIOME_HPP

#include <memory>
namespace ofxPlanet {

class BlockTable;
class World;
class Biome {
       public:
        Biome() = default;
        virtual ~Biome() = default;
        virtual void generate(BlockTable& blockTable) = 0;

       private:
};
}  // namespace ofxPlanet
#endif
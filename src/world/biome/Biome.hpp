#pragma once
#ifndef WORLD_BIOME_BIOME_HPP
#define WORLD_BIOME_BIOME_HPP
#include <ofxImGui.h>

#include <memory>
namespace planet {

class BlockTable;
class World;
class Biome {
       public:
        Biome() = default;
        virtual ~Biome() = default;
        virtual void generate(BlockTable& blockTable) = 0;

       private:
};
}  // namespace planet
#endif
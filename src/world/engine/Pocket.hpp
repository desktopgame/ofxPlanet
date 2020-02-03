#pragma once
#ifndef WORLD_ENGINE_POCKET_HPP
#define WORLD_ENGINE_POCKET_HPP
namespace planet {
class Pocket {
       public:
        explicit Pocket(int x, int y, int z, float zNoise);
        int x;
        int y;
        int z;
        float zNoise;
};
}  // namespace planet
#endif
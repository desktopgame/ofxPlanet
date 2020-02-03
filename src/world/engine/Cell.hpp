#ifndef GAME_WORLD_GEN_CELL_HPP
#define GAME_WORLD_GEN_CELL_HPP
namespace ofxPlanet {
struct Cell {
        int x;
        int z;
        float noise;
        Cell(int x, int z, float noise);
};

bool operator==(const Cell& a, const Cell& b);
}  // namespace ofxPlanet
#endif
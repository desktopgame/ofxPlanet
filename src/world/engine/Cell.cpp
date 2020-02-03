#include "Cell.hpp"
namespace planet {
Cell::Cell(int x, int z, float noise) : x(x), z(z), noise(noise) {}
bool operator==(const Cell& a, const Cell& b) {
        return a.x == b.x && a.z == b.z && a.noise == b.noise;
}
}  // namespace planet

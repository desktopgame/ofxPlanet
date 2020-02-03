#include "IntVec3Compare.hpp"
namespace planet {
bool KeyCompare::operator()(const glm::ivec3& a, const glm::ivec3& b) const {
        return a.x < b.x || (a.x == b.x && a.y < b.y) ||
               (a.x == b.x && a.y == b.y && a.z < b.z);
}
}  // namespace planet
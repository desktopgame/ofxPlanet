#include "IntVec2Hash.hpp"
namespace ofxPlanet {
size_t IntVec2Hash::operator()(const glm::ivec2& k) const {
        return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
}
bool IntVec2Hash::operator()(const glm::ivec2& a, const glm::ivec2& b) const {
        return a.x == b.x && a.y == b.y;
}
}  // namespace ofxPlanet
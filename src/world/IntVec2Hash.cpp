#include "IntVec2Hash.hpp"
namespace planet {
size_t Vec2HashFunc::operator()(const glm::ivec2 & k) const {
	return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
}
bool Vec2HashFunc::operator()(const glm::ivec2 & a, const glm::ivec2 & b) const {
	return a.x == b.x && a.y == b.y;
}
}
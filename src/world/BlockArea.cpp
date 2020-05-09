#include "BlockArea.hpp"
#include <algorithm>

namespace ofxPlanet {
BlockArea::BlockArea() : points() {}
void BlockArea::addPoint(glm::ivec3 point) { points.emplace_back(point); }
glm::ivec3 BlockArea::getPoint(int i) const { return points.at(i); }
int BlockArea::getPointCount() const { return static_cast<int>(points.size()); }
std::vector<glm::ivec3> BlockArea::getPoints() const { return points; }
glm::ivec3 BlockArea::compute2DSize() const {
	glm::ivec3 max(-32768, 0, -32768);
	glm::ivec3 min(32768, 0, 32768);
	for (auto& point : points) {
		min.x = std::min(min.x, point.x);
		min.z = std::min(min.z, point.z);
		max.x = std::max(max.x, point.x);
		max.z = std::max(max.z, point.z);
	}
	return max - min;
}
}
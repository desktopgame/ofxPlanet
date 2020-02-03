#pragma once
#ifndef WORLD_INTVEC3COMPARE_HPP
#define WORLD_INTVEC3COMPARE_HPP
#include <glm/glm.hpp>

namespace ofxPlanet {
struct KeyCompare {
        bool operator()(const glm::ivec3& a, const glm::ivec3& b) const;
};
}  // namespace ofxPlanet
#endif
#pragma once
#ifndef WORLD_INTVEC2HASH_HPP
#define WORLD_INTVEC2HASH_HPP
#include <glm/glm.hpp>

namespace planet {
struct Vec2HashFunc {
	size_t operator()(const glm::ivec2& k) const;
	bool operator()(const glm::ivec2& a, const glm::ivec2& b) const;
};
}
#endif
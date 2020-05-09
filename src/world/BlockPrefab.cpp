#include "BlockPrefab.hpp"
namespace ofxPlanet {
BlockPrefab::BlockPrefab(int id, bool instanced)
	: id(id), instanced(instanced) {}

BlockPrefab::BlockPrefab() : id(-1), instanced(false) {}
}
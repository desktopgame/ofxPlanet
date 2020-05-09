#ifndef WORLD_BLOCKPREFAB_HPP
#define WORLD_BLOCKPREFAB_HPP
namespace ofxPlanet {
/**
 * BlockPrefab is information for generate block to world.
 */
struct BlockPrefab {
	explicit BlockPrefab(int id, bool instanced);
	explicit BlockPrefab();
	int id;
	bool instanced;
};
}
#endif
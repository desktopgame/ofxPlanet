#pragma once
#ifndef WORLD_BIOME_BASICBIOME_HPP
#define WORLD_BIOME_BASICBIOME_HPP
#include <glm/glm.hpp>
#include <unordered_map>

#include "../BlockTable.hpp"
#include "../IntVec2Hash.hpp"
#include "../MultiBlock.hpp"
#include "Biome.hpp"
#include "Random.hpp"
namespace ofxPlanet {

class Block;
/**
 * BasicBiome is base class for implement terrain generation by perlin noise.
 */
class BasicBiome : public Biome {
       public:
        explicit BasicBiome();
        virtual ~BasicBiome();
        void generate(BlockTable& blockTable) override;

       protected:
		/**
		 * return true, if using callbacks.
		 */
        virtual bool isUseCallbacks();
		/**
		 * called on after generate perlin noise.
		 * filtering a got y-axis noise
		 * @param y
		 * @return
		 */
        virtual float onFixHeight(float y);
		/**
		 * called on begin world generate.
		 * @param blockTable
		 */
        virtual void onBeginGenerateWorld(BlockTable& blockTable);
		/**
		 * called on end world generate.
		 * @param blockTable.
		 */
        virtual void onEndGenerateWorld(BlockTable& blockTable);

		/**
		 * called on begin terrain generate.
		 */
        virtual void onBeginGenerateTerrain();
		/**
		 * called on after generate perlin noise.
		 * @param blockTable
		 * @param x
		 * @param y
		 * @param z
		 */
        virtual void onGenerateTerrain(BlockTable& blockTable, int x, int y,
                                       int z);
		/**
		 * called on after generate terrain.
		 */
        virtual void onEndGenerateTerrain();
		/**
		 * called on after generate terrain.
		 * @param blockTable
		 */
        virtual void onGenerateStructures(BlockTable& blockTable);
		/**
		 * called on after generate structure.
		 * @param blockTable
		 * @param x
		 * @param y
		 * @param z
		 * @param noise
		 */
        virtual void onGenerateCave(BlockTable& blockTable, int x, int y, int z,
                                    float noise);

        virtual BlockPrefab createTopBlock(BlockTable& blockTable, int x, int y,
                                           int z) const;
        virtual BlockPrefab createFillBlock(BlockTable& blockTable, int startY,
                                            int x, int y, int z) const;
        virtual BlockPrefab createWaterBlock(BlockTable& blockTable, int x,
                                             int y, int z) const;
        Random random;
        std::unordered_map<glm::ivec2, int, Vec2HashFunc, Vec2HashFunc>
            heightMap;
        char topBlock[255];
        char fillBlock[255];
        char fillHardBlock[255];
        char waterBlock[255];
        bool generateCave;
};
}  // namespace ofxPlanet
#endif
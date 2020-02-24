#pragma once
#ifndef WORLD_BIOME_BASICBIOME_HPP
#define WORLD_BIOME_BASICBIOME_HPP
#include <ofVec3f.h>

#include <unordered_map>

#include "../BlockTable.hpp"
#include "../IntVec2Hash.hpp"
#include "../MultiBlock.hpp"
#include "../Random.hpp"
#include "../WeightTable.hpp"
#include "Biome.hpp"
namespace ofxPlanet {
using HeightMap =
    std::unordered_map<glm::ivec2, int, IntVec2Hash, IntVec2Hash>;
using MultiBlockMap = std::unordered_map<std::string, MultiBlock>;
using WeightTableMap = std::unordered_map<std::string, WeightTable>;

class Block;
class BasicBiome : public Biome {
       public:
        explicit BasicBiome();
        virtual ~BasicBiome();
        void generate(BlockTable& blockTable) override;

        void registerStruct(const std::string& name, const MultiBlock& mb);
		void registerStruct(const std::string& name, const std::string& csvr);
        void generateStruct(BlockTable& table, const std::string& name,
                            int addWeight, int limitWeight);
        void setWeightRange(const std::string& name, glm::ivec3 min,
                            glm::ivec3 max, int weight);
        void setWeight(const std::string& name, int x, int y, int z,
                       int weight);
        int getWeight(const std::string& name, int x, int y, int z);
        MultiBlock& getMultiBlock(const std::string& name) const;
        WeightTable& getWeightTable(const std::string& name);

       protected:
        virtual bool isUseCallbacks();
        virtual float onFixHeight(float y);
        virtual void onBeginGenerateWorld(BlockTable& blockTable);
        virtual void onEndGenerateWorld(BlockTable& blockTable);

        virtual void onBeginGenerateTerrain(BlockTable& blockTable);
        virtual void onGenerateTerrain(BlockTable& blockTable, int x, int y,
                                       int z);
        virtual void onEndGenerateTerrain(BlockTable& blockTable);

        virtual void onGenerateStructures(BlockTable& blockTable);
        virtual void onGenerateCave(BlockTable& blockTable, int x, int y, int z,
                                    float noise);

       private:
        BlockPrefab createTopBlock(BlockTable& blockTable, int x, int y,
                                   int z) const;
        BlockPrefab createFillBlock(BlockTable& blockTable, int startY, int x,
                                    int y, int z) const;
        BlockPrefab createWaterBlock(BlockTable& blockTable, int x, int y,
                                     int z) const;
        Random random;
        std::string topBlock;
        std::string fillBlock;
        std::string fillHardBlock;
        std::string waterBlock;
        bool generateCave;

        std::shared_ptr<HeightMap> heightMap;
        std::shared_ptr<MultiBlockMap> multiBlockMap;
        std::shared_ptr<WeightTableMap> weightTableMap;

       protected:
		glm::ivec3 worldSize;
};
}  // namespace ofxPlanet
#endif
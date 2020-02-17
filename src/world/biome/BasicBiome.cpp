#include "BasicBiome.hpp"

#include <ofMath.h>

#include <random>

#include "../Math.hpp"
#include "../Block.hpp"
#include "../BlockPack.hpp"
#include "../World.hpp"
#include "../engine/Generator.hpp"
#include "../csvr/Parser.hpp"

namespace ofxPlanet {

BasicBiome::BasicBiome()
    : random(),
      heightMap(),
      topBlock("GrassDirt"),
      fillBlock("Dirt"),
      fillHardBlock("HardStone"),
      waterBlock("Water"),
      multiBlockMap(),
      weightTableMap(),
	 generateCave(true) {}

BasicBiome::~BasicBiome() {}

void BasicBiome::generate(BlockTable& blockTable) {
        std::random_device seed_gen;
        const int XSIZE = blockTable.getXSize();
        const int YSIZE = blockTable.getYSize();
        const int ZSIZE = blockTable.getZSize();
        const int YSIZE_H = YSIZE / 2;
        const int XZ = (XSIZE + ZSIZE) / 2;
        this->worldSize = glm::ivec3(XSIZE, YSIZE, ZSIZE);
        this->heightMap = std::make_shared<HeightMap>();
        this->multiBlockMap = std::make_shared<MultiBlockMap>();
        this->weightTableMap = std::make_shared<WeightTableMap>();
        onBeginGenerateWorld(blockTable);
        if (!this->isUseCallbacks()) {
                onEndGenerateWorld(blockTable);
                return;
        }
        Generator gen(XSIZE, YSIZE, ZSIZE);
        // generate terrain
        auto terrain = gen.generate(seed_gen());
        blockTable.setTerrain(terrain);
        onBeginGenerateTerrain(blockTable);
        for (int i = 0; i < terrain.getCellCount(); i++) {
                Cell cellSrc = terrain.getCellAt(i);
                Cell cell =
                    Cell(cellSrc.x, cellSrc.z, onFixHeight(cellSrc.noise));
                int y = YSIZE_H + Math::floatToInt(cell.noise * (YSIZE_H - 1));
                y = std::min(YSIZE - 1, y);
				y = std::max(0, y);
                heightMap->insert_or_assign(glm::ivec2(cell.x, cell.z), y);
                onGenerateTerrain(blockTable, cell.x, y, cell.z);
        }
        onEndGenerateTerrain(blockTable);
        // generate structure
        onGenerateStructures(blockTable);
        // generate cave
        if (this->generateCave) {
                for (int i = 0; i < terrain.getPocketCount(); i++) {
                        Pocket pocket = terrain.getPocketAt(i);
                        onGenerateCave(blockTable, pocket.x, pocket.y, pocket.z,
                                       pocket.zNoise);
                }
        }
        onEndGenerateWorld(blockTable);
}

WeightTable& BasicBiome::getWeightTable(const std::string& name) {
        if (!this->weightTableMap->count(name)) {
                WeightTable wt(this->worldSize.x, this->worldSize.y,
                               this->worldSize.z);
                this->weightTableMap->insert_or_assign(name, wt);
        }
        return weightTableMap->at(name);
}

bool BasicBiome::isUseCallbacks() { return true; }

// protected
float BasicBiome::onFixHeight(float y) { return y; }

void BasicBiome::onBeginGenerateWorld(BlockTable& blockTable) {}

void BasicBiome::onEndGenerateWorld(BlockTable& blockTable) {}

void BasicBiome::onBeginGenerateTerrain(BlockTable& blockTable) {}

void BasicBiome::onGenerateTerrain(BlockTable& blockTable, int x, int y,
                                   int z) {
        blockTable.setBlock(x, y, z, createTopBlock(blockTable, x, y, z));
        int startY = y;
        while (y-- > 0) {
                blockTable.setBlock(x, y, z,
                               createFillBlock(blockTable, startY, x, y, z));
        }
}

void BasicBiome::onEndGenerateTerrain(BlockTable& blockTable) {}

void BasicBiome::onGenerateStructures(BlockTable& blockTable) {}

void BasicBiome::onGenerateCave(BlockTable& blockTable, int x, int y, int z,
                                float noise) {
        if (noise > 0.1f) {
                BlockPrefab pref(-1, false);
                blockTable.setBlock(x, y, z, pref);
        }
}

void BasicBiome::registerStruct(const std::string& name, const MultiBlock& mb) {
        this->multiBlockMap->insert_or_assign(name, mb);
}

void BasicBiome::registerStruct(const std::string & name, const std::string & csvr) {
	MultiBlock mb;
	csvr::Parser parser;
	parser.parse(csvr);
	for (int i = 0; i < parser.getTableCount(); i++) {
		auto& table = parser.getTableAt(i);
		MultiBlockLayer mbLayer;
		for (int j = 0; j < static_cast<int>(table.size()); j++) {
			auto& line = table.at(j);
			MultiBlockLine mbLine;
			for (int k = 0; k < static_cast<int>(line.size());
				k++) {
				auto& col = line.at(k);
				mbLine.emplace_back(col);
			}
			mbLayer.emplace_back(mbLine);
		}
		mb.emplace_back(mbLayer);
	}
	registerStruct(name, mb);
}

void BasicBiome::generateStruct(BlockTable& table, const std::string& name,
                                int addWeight, int limitWeight) {
        // 構造物のだいたいの大きさを取得する
        auto& mb = getMultiBlock(name);
        glm::ivec3 mbSize;
        multiBlock3DSize(mb, mbSize);
        auto& wtable = getWeightTable(name);
        // 全てのエリアに対して
        auto blockAreaVec = table.getAllBlockAreaForTop();
        for (auto& blockArea : blockAreaVec) {
                auto areaSize = blockArea.compute2DSize();
                // 高さが足りないので次へ
                int stackHeight = table.getStackableHeight(blockArea);
                if (stackHeight < mbSize.y) {
                        continue;
                }
                areaSize.y = stackHeight;
                // 幅がたりないので次へ
                if (areaSize.x < mbSize.x || areaSize.z < mbSize.z) {
                        continue;
                }
                std::vector<glm::ivec3> expandPosVec;
                // 一ますごとに配置可能か検証する
                for (int i = 0; i < blockArea.getPointCount(); i++) {
                        auto point = blockArea.getPoint(i);
                        point += glm::ivec3(0, 1, 0);
                        bool canPlace =
                            table.canExpand(point.x, point.y, point.z, mb);
                        if (canPlace) {
                                expandPosVec.emplace_back(point);
                        }
                }
                // シャッフルしてから適当につっこむ
                std::random_device seed_gen;
                std::mt19937 engine(seed_gen());
                std::shuffle(expandPosVec.begin(), expandPosVec.end(), engine);
                for (auto point : expandPosVec) {
                        auto expandVec =
                            table.expandTargets(point.x, point.y, point.z, mb);
                        bool canPlace = true;
                        // 重み付けによって判定する
                        for (auto& expandBlock : expandVec) {
                                glm::ivec3 expandPos = std::get<0>(expandBlock);
                                int weight = wtable.getWeight(
                                    expandPos.x, expandPos.y, expandPos.z);
                                if (limitWeight <= weight) {
                                        canPlace = false;
                                        break;
                                }
                        }
                        if (!canPlace) {
                                continue;
                        }
                        // 展開する
                        table.expand(point.x, point.y, point.z, mb);
                        // 中心の位置を取得
                        auto expandCenter = point;
                        expandCenter.x += mbSize.x / 2;
                        expandCenter.y += mbSize.y / 2;
                        expandCenter.z += mbSize.z / 2;
                        // 中心から重み付けを加算する
                        wtable.addWeight(expandCenter.x, expandCenter.y,
                                         expandCenter.z, addWeight);
                }
        }
}

void BasicBiome::setWeightRange(const std::string& name, glm::ivec3 min,
                                glm::ivec3 max, int weight) {
        auto& wt = getWeightTable(name);
        for (int x = min.x; x <= max.x; x++) {
                for (int y = min.y; y <= max.y; y++) {
                        for (int z = min.z; z <= max.z; z++) {
                                wt.setWeight(x, y, z, weight);
                        }
                }
        }
}

void BasicBiome::setWeight(const std::string& name, int x, int y, int z,
                           int weight) {
        auto& wt = getWeightTable(name);
        wt.setWeight(x, y, z, weight);
}

int BasicBiome::getWeight(const std::string& name, int x, int y, int z) {
        auto& wt = getWeightTable(name);
        return wt.getWeight(x, y, z);
}

MultiBlock& BasicBiome::getMultiBlock(const std::string& name) const {
        return this->multiBlockMap->at(name);
}

// private
BlockPrefab BasicBiome::createTopBlock(BlockTable& blockTable, int x, int y,
                                       int z) const {
        return BlockPrefab(
            BlockPack::getCurrent()->getBlockIndexForName(topBlock), false);
}

BlockPrefab BasicBiome::createFillBlock(BlockTable& blockTable, int startY,
                                        int x, int y, int z) const {
        if (y < (startY - 10)) {
                return BlockPrefab(
                    BlockPack::getCurrent()->getBlockIndexForName(
                        fillHardBlock),
                    false);
        } else {
                return BlockPrefab(
                    BlockPack::getCurrent()->getBlockIndexForName(fillBlock),
                    false);
        }
}

BlockPrefab BasicBiome::createWaterBlock(BlockTable& blockTable, int x, int y,
                                         int z) const {
        return BlockPrefab(
            BlockPack::getCurrent()->getBlockIndexForName(waterBlock), false);
}
}  // namespace ofxPlanet

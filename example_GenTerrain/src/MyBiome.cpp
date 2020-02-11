#include "MyBiome.hpp"

MyBiome::MyBiome() {
}

void MyBiome::onBeginGenerateWorld(ofxPlanet::BlockTable & blockTable) {
	ofxPlanet::MultiBlock mb;
	this->registerStruct("Tree", 
R"(
,,
,Wood,
,,

,,
,Wood,
,,

,,
,Wood,
,,

,,
,Wood,
,,

Leaf,Leaf,Leaf
Leaf,Leaf,Leaf
Leaf,Leaf,Leaf

Leaf,Leaf,Leaf
Leaf,Leaf,Leaf
Leaf,Leaf,Leaf

Leaf,Leaf,Leaf
Leaf,Leaf,Leaf
Leaf,Leaf,Leaf
)");
}

void MyBiome::onGenerateStructures(ofxPlanet::BlockTable & blockTable) {
	int y = static_cast<int>(this->worldSize.y * 0.3f);
	setWeightRange("Tree", glm::ivec3(0, 0, 0), glm::ivec3(worldSize.x-1, y+1, worldSize.z - 1), 10);
	generateStruct(blockTable, "Tree", 12, 1);
}

float MyBiome::onFixHeight(float y) {
	if (y >= -0.2 && y < -0.5)
		y = -0.2;
	else if (y >= -0.5 && y < -0.8)
		y = -0.5;
	else if (y >= -0.8 && y < 0)
		y = -0.8;
	else if (y >= 0 && y < 0.3)
		y = 0;
	else if (y >= 0.3 && y < 0.5)
		y = 0.3;
	else if (y >= 0.5 && y < 0.6)
		y = 0.5;
	else if (y >= 0.6 && y < 0.7)
		y = 0.6;
	else if (y >= 0.7 && y < 0.8)
		y = 0.7;
	else if (y >= 0.8)
		y = 0.8;
	return y;
}

void MyBiome::onGenerateCave(ofxPlanet::BlockTable & blockTable, int x, int y, int z, float noise) {
}

void MyBiome::onEndGenerateTerrain(ofxPlanet::BlockTable& blockTable) {
	int y = static_cast<int>(this->worldSize.y * 0.3f);
	int id = ofxPlanet::BlockPack::getCurrent()->getBlockIndexForName("Water");
	for (int x = 0; x < worldSize.x; x++) {
		for (int z = 0; z < worldSize.z; z++) {
			for (int i = 0; i < y; i++) {
				blockTable.setBlock(x, i, z, ofxPlanet::BlockPrefab(id, false));
			}
		}
	}
}

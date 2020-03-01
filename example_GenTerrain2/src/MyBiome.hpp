#pragma once
#ifndef MYBIOME_HPP
#define MYBIOME_HPP
#include "ofxPlanet.h"

class MyBiome : public ofxPlanet::BasicBiome {
       public:
        MyBiome();

       protected:
        void onBeginGenerateWorld(ofxPlanet::BlockTable& blockTable) override;
        void onGenerateStructures(ofxPlanet::BlockTable& blockTable) override;
        float onFixHeight(float y) override;
        void onGenerateCave(ofxPlanet::BlockTable& blockTable, int x, int y,
                            int z, float noise) override;
        void onEndGenerateTerrain(ofxPlanet::BlockTable& blockTable) override;

       private:
};
#endif
#ifndef WORLD_FLEXIBLEWORLD_HPP
#define WORLD_FLEXIBLEWORLD_HPP
#include <ofShader.h>

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

#include "IWorld.hpp"

namespace ofxPlanet {
class Chunk;
class Block;
class Biome;
class Sector;

/**
 * World is extensible in dynamic.
 * use this class, if want world generate system like minecraft.
 */
class World : public IWorld {
       public:
        using Instance = std::shared_ptr<World>;
        using Reference = std::weak_ptr<World>;
        static Instance create(ofShader& shader, int worldYSize);
        // IWorld
        void computeBrightness();
        int getYSize() const;
        std::shared_ptr<Block> getBlock(int x, int y, int z) const;
        bool isFilled(int x, int y, int z) const;
        int getBrightness(int x, int y, int z) const;
        ofShader& getShader();
        glm::vec3 getViewPosition() const;
        int getViewRange() const;
        ChunkLoadStyle getChunkLoadStyle() const;
        std::shared_ptr<Sector> getSector(int xOffset, int zOffset) const;
        std::shared_ptr<Chunk> getCurrentChunk();
		void setSunBrightness(int sunBrightness);
		int getSunBrightness() const;

        // World
        void invalidateBrightness();
        void setViewPosition(const glm::vec3& viewPosition);
        void setBiome(std::shared_ptr<Biome> biome);
        std::shared_ptr<Biome> getBiome() const;
        void setLoadRange(int loadRange);
        int getLoadRange() const;
        void setVisibleChunkCount(int visibleChunkCount);
        int getVisibleChunkCount() const;
        std::shared_ptr<Sector> getSectorAt(int index) const;
        int getSectorCount() const;
        std::shared_ptr<Chunk> findChunk(int x, int z) const;
        std::shared_ptr<Chunk> loadChunk(int x, int z);
        std::shared_ptr<Chunk> loadChunk(int x, int z, bool& isCreatedNewChunk);
        std::shared_ptr<Chunk> loadOrGenChunk(int x, int z);
        void draw();

       private:
        World(ofShader& shader, int worldYSize);
        std::shared_ptr<Sector> findChunkImpl(int x, int z) const;
        std::shared_ptr<Sector> loadChunkImpl(int x, int z,
                                              bool& isCreatedNewChunk);
        std::shared_ptr<Chunk> loadOrGenChunkImpl(int x, int z, int xOffset,
                                                  int zOffset);
        std::shared_ptr<Chunk> loadOrGenChunkRange(int x, int z, int xOffset,
                                                   int zOffset);
        int computeChunkOffsetX(int x) const;
        int computeChunkOffsetZ(int z) const;
        void updateNeighborChunks();
        int computeGridX(int x) const;
        int computeGridZ(int z) const;

        int worldYSize;
        int chunkXSize;
        int chunkZSize;
        int viewRange;
        int loadRange;
        int visibleChunkCount;
        glm::vec3 viewPosition;
        std::vector<std::shared_ptr<Sector> > sectorVec;
        ofShader& shader;
        std::shared_ptr<Chunk> currentChunk;
        std::shared_ptr<Biome> biome;
		int sunBrightness;
};
}  // namespace ofxPlanet
#endif
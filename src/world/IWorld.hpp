#pragma once
#ifndef WORLD_IWORLD_HPP
#define WORLD_IWORLD_HPP
#include <memory>
#include <ofShader.h>
#include <glm/glm.hpp>
#include "ChunkLoadStyle.hpp"
#include "LightTable.hpp"

namespace ofxPlanet {
class Block;
class Chunk;
class IWorld : public std::enable_shared_from_this<IWorld> {
public:
	using Instance = std::shared_ptr<IWorld>;
	using Reference = std::weak_ptr<IWorld>;
	explicit IWorld() = default;
	virtual ~IWorld(){}
	
	virtual void computeBrightness() = 0;
	virtual int getYSize() const = 0;
	virtual std::shared_ptr<Block> getBlock(int x, int y, int z) const = 0;
	virtual bool isFilled(int x, int y, int z) const = 0;
	virtual ofShader& getShader() = 0;
	virtual glm::vec3 getViewPosition() const = 0;
	virtual int getViewRange() const = 0;
	virtual ChunkLoadStyle getChunkLoadStyle() const = 0;
	virtual LightTable& getLightTable() = 0;
	virtual std::shared_ptr<Chunk> getCurrentChunk() = 0;
private:
};
}
#endif
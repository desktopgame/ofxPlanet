#include "GraphicsBatch.hpp"

#include "World.hpp"
namespace ofxPlanet {
GraphicsBatch::GraphicsBatch(const World& world, ofShader& shader)
    : world(world), shader(shader) {}
glm::vec3 GraphicsBatch::getPhysicalPosition(int x, int y, int z) const {
        return world.getPhysicalPosition(x, y, z);
}
}  // namespace ofxPlanet
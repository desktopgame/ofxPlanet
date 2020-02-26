#include "CubeRenderer.hpp"
namespace ofxPlanet {

CubeRenderer::CubeRenderer(ofShader& shader,
                           const glm::vec3& size, int direction)
    : GraphicsRenderer(shader), size(size), direction(direction) {}

std::shared_ptr<GraphicsBatch> CubeRenderer::createBatch(ofShader& shader) {
        return std::make_shared<CubeBatch>(shader, size, direction);
}
}  // namespace ofxPlanet

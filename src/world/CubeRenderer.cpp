#include "CubeRenderer.hpp"
namespace planet {

CubeRenderer::CubeRenderer(const World& world, ofShader& shader,
                           const glm::vec3& size, int direction)
    : GraphicsRenderer(world, shader), size(size), direction(direction) {}

std::shared_ptr<GraphicsBatch> CubeRenderer::createBatch(const World& world,
                                                         ofShader& shader) {
        return std::make_shared<CubeBatch>(world, shader, size, direction);
}
}  // namespace planet

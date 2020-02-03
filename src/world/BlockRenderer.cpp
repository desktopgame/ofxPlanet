#include "BlockRenderer.hpp"
namespace planet {
BlockRenderer::BlockRenderer(const World& world, ofShader& shader)
    : cubeRenderer(world, shader, glm::vec3(1, 1, 1), 1),
      leftSlabRenderer(world, shader, glm::vec3(0.5f, 1, 1), -1),
      rightSlabRenderer(world, shader, glm::vec3(0.5f, 1, 1), 1),
      frontSlabRenderer(world, shader, glm::vec3(1, 1, 0.5f), -1),
      backSlabRenderer(world, shader, glm::vec3(1, 1, 0.5f), 1),
      topSlabRenderer(world, shader, glm::vec3(1, 0.5f, 1), 1),
      bottomSlabRenderer(world, shader, glm::vec3(1, 0.5f, 1), -1) {}
void BlockRenderer::clear() { cubeRenderer.clear(); }
void BlockRenderer::update() { cubeRenderer.update(); }
void BlockRenderer::render() {
        cubeRenderer.render();
        leftSlabRenderer.render();
        rightSlabRenderer.render();
        frontSlabRenderer.render();
        backSlabRenderer.render();
        topSlabRenderer.render();
        bottomSlabRenderer.render();
}
CubeRenderer& BlockRenderer::getCubeRenderer() { return cubeRenderer; }
CubeRenderer& BlockRenderer::getLeftSlabRenderer() { return leftSlabRenderer; }
CubeRenderer& BlockRenderer::getRightSlabRenderer() {
        return rightSlabRenderer;
}
CubeRenderer& BlockRenderer::getFrontSlabRenderer() {
        return frontSlabRenderer;
}
CubeRenderer& BlockRenderer::getBackSlabRenderer() { return backSlabRenderer; }
CubeRenderer& BlockRenderer::getTopSlabRenderer() { return topSlabRenderer; }
CubeRenderer& BlockRenderer::getBottomSlabRenderer() {
        return bottomSlabRenderer;
}
}  // namespace planet
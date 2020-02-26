#include "BlockRenderer.hpp"
namespace ofxPlanet {
BlockRenderer::BlockRenderer(ofShader& shader)
    : cubeRenderer(shader, glm::vec3(1, 1, 1), 1),
      leftSlabRenderer(shader, glm::vec3(0.5f, 1, 1), -1),
      rightSlabRenderer(shader, glm::vec3(0.5f, 1, 1), 1),
      frontSlabRenderer(shader, glm::vec3(1, 1, 0.5f), -1),
      backSlabRenderer(shader, glm::vec3(1, 1, 0.5f), 1),
      topSlabRenderer(shader, glm::vec3(1, 0.5f, 1), 1),
      bottomSlabRenderer(shader, glm::vec3(1, 0.5f, 1), -1) {}
void BlockRenderer::clear() {
        cubeRenderer.clear();
        leftSlabRenderer.clear();
        rightSlabRenderer.clear();
        frontSlabRenderer.clear();
        backSlabRenderer.clear();
        topSlabRenderer.clear();
        bottomSlabRenderer.clear();
}
void BlockRenderer::update() {
        cubeRenderer.update();
        leftSlabRenderer.update();
        rightSlabRenderer.update();
        frontSlabRenderer.update();
        backSlabRenderer.update();
        topSlabRenderer.update();
        bottomSlabRenderer.update();
}
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
}  // namespace ofxPlanet
#include "GraphicsRenderer.hpp"

#include "GraphicsBatch.hpp"

namespace ofxPlanet {
GraphicsRenderer::GraphicsRenderer(ofShader& shader) : shader(shader), map() {}
void GraphicsRenderer::putFront(GLuint texture, int brightness, PositionType x,
                                PositionType y, PositionType z) {
        ref(texture)->putFront(brightness, x, y, z);
}
void GraphicsRenderer::putBack(GLuint texture, int brightness, PositionType x,
                               PositionType y, PositionType z) {
        ref(texture)->putBack(brightness, x, y, z);
}
void GraphicsRenderer::putLeft(GLuint texture, int brightness, PositionType x,
                               PositionType y, PositionType z) {
        ref(texture)->putLeft(brightness, x, y, z);
}
void GraphicsRenderer::putRight(GLuint texture, int brightness, PositionType x,
                                PositionType y, PositionType z) {
        ref(texture)->putRight(brightness, x, y, z);
}
void GraphicsRenderer::putTop(GLuint texture, int brightness, PositionType x,
                              PositionType y, PositionType z) {
        ref(texture)->putTop(brightness, x, y, z);
}
void GraphicsRenderer::putBottom(GLuint texture, int brightness, PositionType x,
                                 PositionType y, PositionType z) {
        ref(texture)->putBottom(brightness, x, y, z);
}
void GraphicsRenderer::clear() {
        for (auto& kv : map) {
                kv.second->clear();
        }
}
void GraphicsRenderer::update() {
        for (auto& kv : map) {
                kv.second->update();
        }
}
void GraphicsRenderer::render() {
        for (auto& kv : map) {
                kv.second->render(kv.first);
        }
}
std::shared_ptr<GraphicsBatch> GraphicsRenderer::ref(GLuint texture) {
        if (!map.count(texture)) {
                map.insert_or_assign(texture, createBatch(this->shader));
        }
        return map.at(texture);
}
}  // namespace ofxPlanet
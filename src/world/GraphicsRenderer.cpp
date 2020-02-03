#include "GraphicsRenderer.hpp"

#include "GraphicsBatch.hpp"

namespace planet {
GraphicsRenderer::GraphicsRenderer(const World& world, ofShader& shader)
    : world(world), shader(shader), map() {}
void GraphicsRenderer::putFront(GLuint texture, int x, int y, int z) {
        ref(texture)->putFront(x, y, z);
}
void GraphicsRenderer::putBack(GLuint texture, int x, int y, int z) {
        ref(texture)->putBack(x, y, z);
}
void GraphicsRenderer::putLeft(GLuint texture, int x, int y, int z) {
        ref(texture)->putLeft(x, y, z);
}
void GraphicsRenderer::putRight(GLuint texture, int x, int y, int z) {
        ref(texture)->putRight(x, y, z);
}
void GraphicsRenderer::putTop(GLuint texture, int x, int y, int z) {
        ref(texture)->putTop(x, y, z);
}
void GraphicsRenderer::putBottom(GLuint texture, int x, int y, int z) {
        ref(texture)->putBottom(x, y, z);
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
                map.insert_or_assign(texture,
                                     createBatch(this->world, this->shader));
        }
        return map.at(texture);
}
}  // namespace planet
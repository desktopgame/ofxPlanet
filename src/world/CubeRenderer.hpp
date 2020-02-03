#pragma once
#ifndef WORLD_CUBERENDERER_HPP
#define WORLD_CUBERENDERER_HPP
#include <ofShader.h>

#include <unordered_map>

#include "CubeBatch.hpp"
#include "GraphicsRenderer.hpp"
namespace ofxPlanet {

class Camera;
/**
 * CubeRenderer is implement GraphicsRenderer ufor rendering Planes.
 */
class CubeRenderer : public GraphicsRenderer {
       public:
        explicit CubeRenderer(const World& world, ofShader& shader,
                              const glm::vec3& size, int direction);

       protected:
        std::shared_ptr<GraphicsBatch> createBatch(const World& world,
                                                   ofShader& shader) override;

       private:
        glm::vec3 size;
        int direction;
};
}  // namespace ofxPlanet
#endif
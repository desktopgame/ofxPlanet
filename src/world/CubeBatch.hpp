#pragma once
#ifndef WORLD_CUBEBATCH_HPP
#define WORLD_CUBEBATCH_HPP
#include <ofMatrix4x4.h>
#include <ofShader.h>

#include <array>
#include <memory>
#include <vector>

#include "GraphicsBatch.hpp"
#include "GraphicsRenderer.hpp"
#include "LightTable.hpp"
#include "Plane.hpp"
namespace ofxPlanet {
class World;
class Camera;
/**
 * CubeRenderer is implement GraphicsBatch ufor rendering Planes.
 */
class CubeBatch : public GraphicsBatch {
       public:
        using PlaneArray = std::array<std::shared_ptr<Plane>,
                                      static_cast<int>(PlaneType::Count)>;
        using PosVec = std::vector<PositionType>;
        using PosArray = std::array<PosVec, static_cast<int>(PlaneType::Count)>;
        using VboArray = GLuint[6];
        explicit CubeBatch(ofShader& shader, const glm::vec3& size,
                           int direction);
        ~CubeBatch();

        void putFront(int brightness, PositionType x, PositionType y,
                      PositionType z) override;
        void putBack(int brightness, PositionType x, PositionType y,
                     PositionType z) override;
        void putLeft(int brightness, PositionType x, PositionType y,
                     PositionType z) override;
        void putRight(int brightness, PositionType x, PositionType y,
                      PositionType z) override;
        void putTop(int brightness, PositionType x, PositionType y,
                    PositionType z) override;
        void putBottom(int brightness, PositionType x, PositionType y,
                       PositionType z) override;
        void clear() override;
        void update() override;
        void render(GLuint texture) override;

       private:
        void put(PlaneType type, int brightness, PositionType x, PositionType y,
                 PositionType z);
        void updatePlane(PlaneType type, int brightness);
        std::vector<PositionType>& getPosVec(PlaneType type, int brightness);
        glm::vec3 size;
        int direction;

        bool isInvalid;
        std::array<PlaneArray, LightTable::BRIGHTNESS_MAX + 1> planes;
        std::array<PosArray, LightTable::BRIGHTNESS_MAX + 1> posVec;
        std::array<VboArray, LightTable::BRIGHTNESS_MAX + 1> vbo;
};
}  // namespace ofxPlanet
#endif
#pragma once
#ifndef WORLD_CUBEBATCH_HPP
#define WORLD_CUBEBATCH_HPP
#include <ofMatrix4x4.h>
#include <ofShader.h>

#include <array>
#include <memory>
#include <vector>

#include "GraphicsBatch.hpp"
#include "Plane.hpp"
namespace planet {
class World;
class Camera;
class CubeBatch : public GraphicsBatch {
       public:
        explicit CubeBatch(const World& world, ofShader& shader,
                           const glm::vec3& size, int direction);
        ~CubeBatch();

        void putFront(int x, int y, int z) override;
        void putBack(int x, int y, int z) override;
        void putLeft(int x, int y, int z) override;
        void putRight(int x, int y, int z) override;
        void putTop(int x, int y, int z) override;
        void putBottom(int x, int y, int z) override;
        void clear() override;
        void update() override;
        void render(GLuint texture) override;

       private:
        void put(PlaneType type, int x, int y, int z);
        void updatePlane(PlaneType type);
        std::vector<float>& getPosVec(PlaneType type);
        glm::vec3 size;
        int direction;

        bool isInvalid;
        std::array<std::shared_ptr<Plane>, static_cast<int>(PlaneType::Count)>
            planes;
        std::array<std::vector<float>, static_cast<int>(PlaneType::Count)>
            posVec;
        ofBufferObject vbo[static_cast<int>(PlaneType::Count)];
        // ofShader& shader;
};
}  // namespace planet
#endif
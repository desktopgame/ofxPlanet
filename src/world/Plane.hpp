#pragma once
#ifndef WORLD_PLANE_HPP
#define WORLD_PLANE_HPP
#include <ofShader.h>
#include <ofVbo.h>

namespace planet {
/**
 * PlaneType は平面の方向を設定する列挙型です。
 */
enum class PlaneType : unsigned int {
        Front = 0,
        Back = 1,
        Left = 2,
        Right = 3,
        Top = 4,
        Bottom = 5,
        Count = 6,
};
class Camera;
class Shader;
/**
 * Plane は平面を描画するためのクラスです。
 */
class Plane {
       public:
        explicit Plane(ofShader& shader, PlaneType type, const glm::vec3 size);
        void draw();
        void drawInstanced(int count);

        ofVbo& getVAO();
        const ofVbo& getVAO() const;

       private:
        void setupOfVboData(std::vector<float> vertex,
                            std::vector<float> normal, std::vector<float> uv);
        void setupOfVbo(PlaneType type, const glm::vec3 size);
        Plane(const Plane& obj) = delete;
        Plane& operator=(const Plane&) = delete;

        static std::vector<float> createFrontVertex(glm::vec3 size);
        static std::vector<float> createFrontNormal(glm::vec3 size);
        static std::vector<float> createFrontUV();

        static std::vector<float> createBackVertex(glm::vec3 size);
        static std::vector<float> createBackNormal(glm::vec3 size);
        static std::vector<float> createBackUV();

        static std::vector<float> createLeftVertex(glm::vec3 size);
        static std::vector<float> createLeftNormal(glm::vec3 size);
        static std::vector<float> createLeftUV();

        static std::vector<float> createRightVertex(glm::vec3 size);
        static std::vector<float> createRightNormal(glm::vec3 size);
        static std::vector<float> createRightUV();

        static std::vector<float> createTopVertex(glm::vec3 size);
        static std::vector<float> createTopNormal(glm::vec3 size);
        static std::vector<float> createTopUV();

        static std::vector<float> createBottomVertex(glm::vec3 size);
        static std::vector<float> createBottomNormal(glm::vec3 size);
        static std::vector<float> createBottomUV();

        PlaneType type;
        ofVbo ofVAO;
        ofShader& shader;
};
}  // namespace planet
#endif  // !SHADER_PLANE_HPP

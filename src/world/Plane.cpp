#include "Plane.hpp"

#include "Camera.hpp"
namespace planet {

Plane::Plane(ofShader& shader, PlaneType type, const glm::vec3 size)
    : shader(shader), type(type), ofVAO() {
        setupOfVbo(type, size);
}

void Plane::draw() {
        shader.begin();
        ofVAO.drawElements(GL_TRIANGLES, 6);
        shader.end();
}

void Plane::drawInstanced(int count) {
        shader.begin();
        ofVAO.drawElementsInstanced(GL_TRIANGLES, 6, count);
        shader.end();
}

ofVbo& Plane::getVAO() { return ofVAO; }

const ofVbo& Plane::getVAO() const { return ofVAO; }

void Plane::setupOfVboData(std::vector<float> vertex, std::vector<float> normal,
                           std::vector<float> uv) {
        ofVAO.bind();
        ofVAO.setVertexData(vertex.data(), 3, vertex.size(), GL_STATIC_DRAW);
        ofVAO.setNormalData(normal.data(), normal.size(), GL_STATIC_DRAW);
        ofVAO.setTexCoordData(uv.data(), uv.size(), GL_STATIC_DRAW);
        ofVAO.unbind();
}

void Plane::setupOfVbo(PlaneType type, const glm::vec3 size) {
        switch (type) {
                case PlaneType::Front:
                        setupOfVboData(createFrontVertex(size),
                                       createFrontNormal(size),
                                       createFrontUV());
                        break;
                case PlaneType::Back:
                        setupOfVboData(createBackVertex(size),
                                       createBackNormal(size), createBackUV());
                        break;
                case PlaneType::Left:
                        setupOfVboData(createLeftVertex(size),
                                       createBackNormal(size), createBackUV());
                        break;
                case PlaneType::Right:
                        setupOfVboData(createRightVertex(size),
                                       createRightNormal(size),
                                       createRightUV());
                        break;
                case PlaneType::Top:
                        setupOfVboData(createTopVertex(size),
                                       createTopNormal(size), createTopUV());
                        break;
                case PlaneType::Bottom:
                        setupOfVboData(createBottomVertex(size),
                                       createBottomNormal(size),
                                       createBottomUV());
                        break;
        }
        auto indexData = std::vector<ofIndexType>{0, 1, 2, 2, 3, 0};
        ofVAO.setIndexData(indexData.data(), indexData.size(), GL_STATIC_DRAW);
        /*
                auto shader = nameSet.getShader();
                shader->use();
                nameSet.apply(shader);
                shader->unuse();
        */
}

std::vector<float> Plane::createFrontVertex(glm::vec3 size) {
        return std::vector<float>{
            -size.x, -size.y, size.z, size.x,  -size.y, size.z,
            size.x,  size.y,  size.z, -size.x, size.y,  size.z,
        };
}

std::vector<float> Plane::createFrontNormal(glm::vec3 size) {
        return std::vector<float>{0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1};
}

std::vector<float> Plane::createFrontUV() {
        return std::vector<float>{0, 1, 1, 1, 1, 0, 0, 0};
}

std::vector<float> Plane::createBackVertex(glm::vec3 size) {
        return std::vector<float>{
            size.x,  -size.y, -size.z, -size.x, -size.y, -size.z,
            -size.x, size.y,  -size.z, size.x,  size.y,  -size.z,
        };
}

std::vector<float> Plane::createBackNormal(glm::vec3 size) {
        return std::vector<float>{
            0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1,
        };
}

std::vector<float> Plane::createBackUV() {
        return std::vector<float>{0, 1, 1, 1, 1, 0, 0, 0};
}

std::vector<float> Plane::createLeftVertex(glm::vec3 size) {
        return std::vector<float>{
            -size.x, -size.y, -size.z, -size.x, -size.y, size.z,
            -size.x, size.y,  size.z,  -size.x, size.y,  -size.z,
        };
}

std::vector<float> Plane::createLeftNormal(glm::vec3 size) {
        return std::vector<float>{
            -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0,
        };
}

std::vector<float> Plane::createLeftUV() {
        return std::vector<float>{0, 1, 1, 1, 1, 0, 0, 0};
}

std::vector<float> Plane::createRightVertex(glm::vec3 size) {
        return std::vector<float>{
            size.x, -size.y, size.z,  size.x, -size.y, -size.z,
            size.x, size.y,  -size.z, size.x, size.y,  size.z,
        };
}

std::vector<float> Plane::createRightNormal(glm::vec3 size) {
        return std::vector<float>{
            1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
        };
}

std::vector<float> Plane::createRightUV() {
        return std::vector<float>{0, 1, 1, 1, 1, 0, 0, 0};
}

std::vector<float> Plane::createTopVertex(glm::vec3 size) {
        return std::vector<float>{
            -size.x, size.y, size.z,  size.x,  size.y, size.z,
            size.x,  size.y, -size.z, -size.x, size.y, -size.z,
        };
}

std::vector<float> Plane::createTopNormal(glm::vec3 size) {
        return std::vector<float>{
            0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
        };
}

std::vector<float> Plane::createTopUV() {
        return std::vector<float>{0, 0, 1, 0, 1, 1, 0, 1};
}

std::vector<float> Plane::createBottomVertex(glm::vec3 size) {
        return std::vector<float>{
            -size.x, -size.y, -size.z, size.x,  -size.y, -size.z,
            size.x,  -size.y, size.z,  -size.x, -size.y, size.z,
        };
}

std::vector<float> Plane::createBottomNormal(glm::vec3 size) {
        return std::vector<float>{
            0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0,
        };
}

std::vector<float> Plane::createBottomUV() {
        return std::vector<float>{0, 0, 1, 0, 1, 1, 0, 1};
}
}  // namespace planet

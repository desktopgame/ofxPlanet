#include "CubeBatch.hpp"

#include <ofAppRunner.h>

#include <glm/glm.hpp>

#include "Block.hpp"
#include "UniformLayout.hpp"
#include "VertexLayout.hpp"
namespace ofxPlanet {
CubeBatch::CubeBatch(ofShader& shader,
                     const glm::vec3& size, int direction)
    : GraphicsBatch(shader),
      isInvalid(true),
      size(size),
      direction(direction),
      planes(),
      posVec(),
      vbo() {
        for (int b = LightTable::BRIGHTNESS_MIN;
             b < LightTable::BRIGHTNESS_MAX + 1; b++) {
				glGenBuffers(6, vbo[b]);
                for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
                        std::vector<float> v;
                        planes[b][i] = std::make_shared<Plane>(
                            shader, static_cast<PlaneType>(i), size);
                        posVec[b][i] = v;
                }
        }
}

CubeBatch::~CubeBatch() {
        for (int b = LightTable::BRIGHTNESS_MIN;
             b < LightTable::BRIGHTNESS_MAX + 1; b++) {
				glDeleteBuffers(6, vbo[b]);
        }
}

void CubeBatch::putFront(int brightness, int x, int y, int z) {
        put(PlaneType::Front, brightness, x, y, z);
}

void CubeBatch::putBack(int brightness, int x, int y, int z) {
        put(PlaneType::Back, brightness, x, y, z);
}

void CubeBatch::putLeft(int brightness, int x, int y, int z) {
        put(PlaneType::Left, brightness, x, y, z);
}

void CubeBatch::putRight(int brightness, int x, int y, int z) {
        put(PlaneType::Right, brightness, x, y, z);
}

void CubeBatch::putTop(int brightness, int x, int y, int z) {
        put(PlaneType::Top, brightness, x, y, z);
}

void CubeBatch::putBottom(int brightness, int x, int y, int z) {
        put(PlaneType::Bottom, brightness, x, y, z);
}

void CubeBatch::clear() {
        for (int b = LightTable::BRIGHTNESS_MIN;
             b < LightTable::BRIGHTNESS_MAX + 1; b++) {
                auto& posA = this->posVec[b];
                for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
                        posA.at(i).clear();
                }
        }
        this->isInvalid = true;
}

void CubeBatch::update() {
        if (!isInvalid) {
                return;
        }
        for (int b = LightTable::BRIGHTNESS_MIN;
             b < LightTable::BRIGHTNESS_MAX + 1; b++) {
                for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
                        updatePlane(static_cast<PlaneType>(i), b);
                }
        }
        this->isInvalid = false;
}

void CubeBatch::render(GLuint texture) {
        update();
        glBindTexture(GL_TEXTURE_2D, texture);
        for (int b = LightTable::BRIGHTNESS_MIN;
             b < LightTable::BRIGHTNESS_MAX + 1; b++) {
                float f = LightTable::computeShaderBrightness(b);
                shader.begin();
                shader.setUniform1f(UniformLayout::BRIGHTNESSS_NAME, f);
                auto& planeA = this->planes[b];
                for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
                        GLuint vao = planeA[i]->getVAO();
                        GLuint index = planeA[i]->getIndex();
                        glBindVertexArray(vao);
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
                        glDrawElementsInstanced(
                            GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr,
                            getPosVec(static_cast<PlaneType>(i), b).size());
                }
                shader.end();
        }
}
// private

void CubeBatch::put(PlaneType type, int brightness, int x, int y, int z) {
        glm::vec3 pos = glm::vec3(x, y, z) * 2.0f;
        glm::vec3 baseSize = sizeFromShape(BlockShape::Block);
        glm::vec3 offset = ((baseSize - (this->size * 2.0f)) / 2.0f) *
                           static_cast<float>(direction);
        pos += offset;
        getPosVec(type, brightness).emplace_back(pos.x);
        getPosVec(type, brightness).emplace_back(pos.y);
        getPosVec(type, brightness).emplace_back(pos.z);
        this->isInvalid = true;
}

void CubeBatch::updatePlane(PlaneType type, int brightness) {
        int index = static_cast<int>(type);
        std::vector<float>& posVec = getPosVec(type, brightness);
        if (posVec.empty()) {
                return;
        }
        // update vbo
        GLuint buf = vbo[brightness][index];
        glBindBuffer(GL_ARRAY_BUFFER, buf);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ofIndexType) * posVec.size(),
                     posVec.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        GLuint vao = planes[brightness][index]->getVAO();
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, buf);
        glVertexAttribPointer(VertexLayout::POSITION_POSITION, 3, GL_FLOAT,
                              GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(VertexLayout::POSITION_POSITION);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glVertexAttribDivisor(VertexLayout::VERTEX_POSITION, 0);
        glVertexAttribDivisor(2, 0);
        glVertexAttribDivisor(VertexLayout::TEXCOORD_POSITION, 0);
        glVertexAttribDivisor(VertexLayout::POSITION_POSITION, 1);
        glBindVertexArray(0);
}

std::vector<float>& CubeBatch::getPosVec(PlaneType type, int brightness) {
        int i = static_cast<int>(type);
        assert(i >= 0 && i < static_cast<int>(PlaneType::Count));
        return posVec.at(brightness).at(i);
}
}  // namespace ofxPlanet

#include "CubeBatch.hpp"

#include "../common/GLM.hpp"
#include "Block.hpp"
namespace planet {

CubeBatch::CubeBatch(const World& world, ofShader& shader,
                     const glm::vec3& size, int direction)
    : GraphicsBatch(world, shader),
      isInvalid(true),
      size(size),
      direction(direction),
      planes(),
      posVec(),
      vbo() {
        for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
                std::vector<float> v;
                planes[i] = std::make_shared<Plane>(
                    shader, static_cast<PlaneType>(i), size);
                posVec[i] = v;
                vbo[i].allocate();
        }
}

CubeBatch::~CubeBatch() {}

void CubeBatch::putFront(int x, int y, int z) {
        put(PlaneType::Front, x, y, z);
}

void CubeBatch::putBack(int x, int y, int z) { put(PlaneType::Back, x, y, z); }

void CubeBatch::putLeft(int x, int y, int z) { put(PlaneType::Left, x, y, z); }

void CubeBatch::putRight(int x, int y, int z) {
        put(PlaneType::Right, x, y, z);
}

void CubeBatch::putTop(int x, int y, int z) { put(PlaneType::Top, x, y, z); }

void CubeBatch::putBottom(int x, int y, int z) {
        put(PlaneType::Bottom, x, y, z);
}

void CubeBatch::clear() {
        for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
                posVec.at(i).clear();
        }
        this->isInvalid = true;
}

void CubeBatch::update() {
        if (!isInvalid) {
                return;
        }
        for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
                updatePlane(static_cast<PlaneType>(i));
        }
        this->isInvalid = false;
}

void CubeBatch::render(GLuint texture) {
        update();
        glBindTexture(GL_TEXTURE_2D, texture);

        for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
                planes[i]->drawInstanced(
                    getPosVec(static_cast<PlaneType>(i)).size());
        }
}
// private

void CubeBatch::put(PlaneType type, int x, int y, int z) {
        glm::vec3 pos = getPhysicalPosition(x, y, z);
        glm::vec3 baseSize = sizeFromShape(BlockShape::Block);
        glm::vec3 offset = ((baseSize - (this->size * 2.0f)) / 2.0f) *
                           static_cast<float>(direction);
        pos += offset;
        getPosVec(type).emplace_back(pos.x);
        getPosVec(type).emplace_back(pos.y);
        getPosVec(type).emplace_back(pos.z);
        this->isInvalid = true;
}

void CubeBatch::updatePlane(PlaneType type) {
        int index = static_cast<int>(type);
        std::vector<float>& posVec = getPosVec(type);
        if (posVec.empty()) {
                return;
        }
        // update vbo
        ofBufferObject& v = vbo[index];
        v.bind(GL_ARRAY_BUFFER);
        v.setData(posVec, GL_STATIC_DRAW);
        v.unbind(GL_ARRAY_BUFFER);
        // update vao
        ofVbo& vao = planes[index]->getVAO();
        shader.begin();
        vao.bind();
        // vertex Attributes
        vao.setAttributeBuffer(4, v, 3, 0);
        vao.setAttributeDivisor(0, 0);
        vao.setAttributeDivisor(2, 0);
        vao.setAttributeDivisor(3, 0);
        vao.setAttributeDivisor(4, 1);

        vao.unbind();
        shader.end();
}

std::vector<float>& CubeBatch::getPosVec(PlaneType type) {
        int i = static_cast<int>(type);
        assert(i >= 0 && i < static_cast<int>(PlaneType::Count));
        return posVec.at(i);
}
}  // namespace planet

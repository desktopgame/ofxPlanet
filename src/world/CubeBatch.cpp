#include "CubeBatch.hpp"

#include <glm/glm.hpp>
#include <ofAppRunner.h>

#include "Block.hpp"
namespace ofxPlanet {
int CubeBatch::POSITION_INDEX = 4;
CubeBatch::CubeBatch(const World& world, ofShader& shader,
                     const glm::vec3& size, int direction)
    : GraphicsBatch(world, shader),
      isInvalid(true),
      size(size),
      direction(direction),
	planes({
	PlaneArray{},
	PlaneArray{},
	PlaneArray{},

	PlaneArray{},
	PlaneArray{},
	PlaneArray{},
	PlaneArray{},
	PlaneArray{},
	PlaneArray{},
	PlaneArray{},
	PlaneArray{},

	PlaneArray{},
	PlaneArray{},
	PlaneArray{},
	PlaneArray{},
	PlaneArray{},
	}),
	posVec({
		PosArray{},
		PosArray{},
		PosArray{},

		PosArray{},
		PosArray{},
		PosArray{},
		PosArray{},
		PosArray{},
		PosArray{},
		PosArray{},
		PosArray{},

		PosArray{},
		PosArray{},
		PosArray{},
		PosArray{},
		PosArray{},
	}),
	vbo({
		VboArray{},
		VboArray{},
		VboArray{},

		VboArray{},
		VboArray{},
		VboArray{},
		VboArray{},
		VboArray{},
		VboArray{},
		VboArray{},
		VboArray{},

		VboArray{},
		VboArray{},
		VboArray{},
		VboArray{},
		VboArray{},
	}) {
		for (int b = LightTable::BRIGHTNESS_MIN; b < LightTable::BRIGHTNESS_MAX+1; b++) {
			for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
				std::vector<float> v;
				GLuint buf;
				glGenBuffers(1, &buf);
				planes[b][i] = std::make_shared<Plane>(shader, static_cast<PlaneType>(i), size);
				posVec[b][i] = v;
				vbo[b][i] = buf;
				planes[b][i]->init();
			}
		}
}

CubeBatch::~CubeBatch() {
	for (int b = LightTable::BRIGHTNESS_MIN; b < LightTable::BRIGHTNESS_MAX + 1; b++) {
		for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
			GLuint buf = vbo[b][i];
			glDeleteBuffers(1, &buf);
		}
	}
}

void CubeBatch::putFront(int brightness, int x, int y, int z) {
        put(PlaneType::Front, brightness, x, y, z);
}

void CubeBatch::putBack(int brightness, int x, int y, int z) { put(PlaneType::Back, brightness, x, y, z); }

void CubeBatch::putLeft(int brightness, int x, int y, int z) { put(PlaneType::Left, brightness, x, y, z); }

void CubeBatch::putRight(int brightness, int x, int y, int z) {
        put(PlaneType::Right, brightness, x, y, z);
}

void CubeBatch::putTop(int brightness, int x, int y, int z) { put(PlaneType::Top, brightness, x, y, z); }

void CubeBatch::putBottom(int brightness, int x, int y, int z) {
        put(PlaneType::Bottom, brightness, x, y, z);
}

void CubeBatch::clear() {
	for (int b = LightTable::BRIGHTNESS_MIN; b < LightTable::BRIGHTNESS_MAX+1; b++) {
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
		for (int b = LightTable::BRIGHTNESS_MIN; b < LightTable::BRIGHTNESS_MAX+1; b++) {
			for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
				updatePlane(static_cast<PlaneType>(i), b);
			}
		}
        this->isInvalid = false;
}

void CubeBatch::render(GLuint texture) {
        update();
        glBindTexture(GL_TEXTURE_2D, texture);
		for (int b = LightTable::BRIGHTNESS_MIN; b < LightTable::BRIGHTNESS_MAX+1; b++) {
			float f = LightTable::computeShaderBrightness(b);
			shader.begin();
			shader.setUniform1f(LightTable::BRIGHTNAME_UNIFORM_NAME, f);
			auto& planeA = this->planes[b];
			for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
				GLuint vao = planeA[i]->getVAO();
				GLuint index = planeA[i]->getIndex();
				glBindVertexArray(vao);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
				glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr,getPosVec(static_cast<PlaneType>(i), b).size());
				//planeA[i]->drawInstanced(
				//	getPosVec(static_cast<PlaneType>(i), b).size());
			}
			shader.end();
		}
}
// private

void CubeBatch::put(PlaneType type, int brightness, int x, int y, int z) {
        glm::vec3 pos = getPhysicalPosition(x, y, z);
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(ofIndexType) * posVec.size(), posVec.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//v.bind(GL_ARRAY_BUFFER);
        //v.setData(posVec, GL_STATIC_DRAW);
		//v.unbind(GL_ARRAY_BUFFER);
        // update vao
        GLuint vao = planes[brightness][index]->getVAO();
        // vertex Attributes
		//shader.begin();
		//vao.bind();
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, buf);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glVertexAttribDivisor(0, 0);
		glVertexAttribDivisor(2, 0);
		glVertexAttribDivisor(3, 0);
		glVertexAttribDivisor(4, 1);
		glBindVertexArray(0);
        //vao.setAttributeBuffer(POSITION_INDEX, v, 3, 0);
        //vao.setAttributeDivisor(0, 0);
        //vao.setAttributeDivisor(2, 0);
        //vao.setAttributeDivisor(3, 0);
        //vao.setAttributeDivisor(POSITION_INDEX, 1);
		//vao.unbind();
		//shader.end();
}

std::vector<float>& CubeBatch::getPosVec(PlaneType type, int brightness) {
        int i = static_cast<int>(type);
        assert(i >= 0 && i < static_cast<int>(PlaneType::Count));
        return posVec.at(brightness).at(i);
}
}  // namespace ofxPlanet

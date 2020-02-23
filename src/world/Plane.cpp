#include "Plane.hpp"

namespace ofxPlanet {
glm::vec3 Plane::TOP_NORMAL = glm::vec3(0, 1, 0);
glm::vec3 Plane::BOTTOM_NORMAL = glm::vec3(0, -1, 0);
glm::vec3 Plane::LEFT_NORMAL = glm::vec3(-1, 0, 0);
glm::vec3 Plane::RIGHT_NORMAL = glm::vec3(1, 0, 0);
glm::vec3 Plane::FRONT_NORMAL = glm::vec3(0, 0, 1);
glm::vec3 Plane::BACK_NORMAL = glm::vec3(0, 0, -1);
int Plane::VERTEX_INDEX = 0;
int Plane::NORMAL_INDEX = 2;
int Plane::TEXCOORD_INDEX = 3;

Plane::Plane(ofShader& shader, PlaneType type, const glm::vec3 size)
    : shader(shader), type(type), size(size) {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vertexBuf);
	glGenBuffers(1, &texcoordBuf);
	glGenBuffers(1, &indexBuf);
	setupOfVbo(type, size);
}

Plane::~Plane() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vertexBuf);
	glDeleteBuffers(1, &texcoordBuf);
	glDeleteBuffers(1, &indexBuf);
}

GLuint Plane::getVAO() const { return vao; }

GLuint Plane::getIndex() const { return indexBuf; }

void Plane::setupOfVboData(std::vector<float> vertex, std::vector<float> normal,
                           std::vector<float> uv) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex.size(), vertex.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, texcoordBuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uv.size(), uv.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Plane::setupOfVbo(PlaneType type, const glm::vec3 size) {
		glBindVertexArray(vao);
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
                                       createLeftNormal(size), createLeftUV());
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
				default:
					throw std::logic_error("");
        }
        auto indexData = std::vector<ofIndexType>{0, 1, 2, 2, 3, 0};
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ofIndexType) * indexData.size(), indexData.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
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
}  // namespace ofxPlanet

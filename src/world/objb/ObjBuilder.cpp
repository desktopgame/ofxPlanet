#include "ObjBuilder.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace planet {
namespace objb {
// ObjIndex
ObjIndex::ObjIndex(int index, IndexMode mode)
    : index(index), mode(mode), valid(true) {}
ObjIndex::ObjIndex(int index)
    : index(index), mode(IndexMode::Local), valid(true) {}
ObjIndex::ObjIndex() : index(0), mode(IndexMode::Local), valid(false) {}
// ObjPolygon
ObjPolygon::ObjPolygon(ObjIndex vertexIndex, ObjIndex texcoordIndex,
                       ObjIndex normalIndex)
    : vertexIndex(vertexIndex),
      texcoordIndex(texcoordIndex),
      normalIndex(normalIndex) {}
ObjPolygon::ObjPolygon(ObjIndex vertexIndex, ObjIndex texcoordIndex)
    : vertexIndex(vertexIndex), texcoordIndex(texcoordIndex), normalIndex() {}
// ObjModel
ObjModel::ObjModel(ObjBuilder& builder, const std::string& name)
    : vertexCount(0),
      useIndexCount(0),
      builder(builder),
      name(name),
      vertices(),
      normals(),
      texcoords() {}
ObjModel& ObjModel::sharedVertex(const glm::vec3& aVertex,
                                 ObjPolygon& destPoly) {
        vertices.emplace_back(aVertex);
        destPoly.vertexIndex.index = builder.countVertex();
        destPoly.vertexIndex.mode = IndexMode::Global;
        this->vertexCount++;
        return *this;
}
ObjModel& ObjModel::vertex(const glm::vec3& vertex) {
        vertices.emplace_back(vertex);
        this->vertexCount++;
        return *this;
}
ObjModel& ObjModel::normal(const glm::vec3& normal) {
        normals.emplace_back(normal);
        return *this;
}
ObjModel& ObjModel::texcoord(const glm::vec2& texcoord) {
        texcoords.emplace_back(texcoord);
        return *this;
}
ObjModel& ObjModel::face(const ObjFace& face) {
        faces.emplace_back(face);
        return *this;
}
ObjModel& ObjModel::useMaterial(const std::string& material) {
        this->material = material;
        return *this;
}
int ObjModel::getUseIndexCount() { return useIndexCount; }
// ObjBuilder
ObjBuilder::ObjBuilder() : models(), allVertexCount(0) {}
ObjBuilder::~ObjBuilder() {
        for (auto model : models) {
                delete model;
        }
        models.clear();
}
ObjModel& ObjBuilder::newModel(const std::string& name) {
        auto model = new ObjModel(*this, name);
        models.emplace_back(model);
        return *model;
}
void ObjBuilder::reserveModels(int size) { models.reserve(size); }
ObjBuilder& ObjBuilder::material(const std::string& _material) {
        this->_material = _material;
        return *this;
}
ObjBuilder& ObjBuilder::globalVertex(glm::vec3 vertex) {
        vertices.emplace_back(vertex);
        return *this;
}
ObjBuilder& ObjBuilder::globalNormal(glm::vec3 normal) {
        normals.emplace_back(normal);
        return *this;
}
ObjBuilder& ObjBuilder::globalTexcoord(glm::vec2 texcoord) {
        texcoords.emplace_back(texcoord);
        return *this;
}
std::string ObjBuilder::toString() const {
        std::stringstream ss;
        write(ss);
        return ss.str();
}
ObjModel& ObjBuilder::getModelAt(int index) { return *models.at(index); }
int ObjBuilder::getModelCount() const {
        return static_cast<int>(models.size());
}
int ObjBuilder::getGlobalVertexCount() const {
        return static_cast<int>(vertices.size());
}
int ObjBuilder::getGlobalNormalCount() const {
        return static_cast<int>(normals.size());
}
int ObjBuilder::getGloalTexcoordCount() const {
        return static_cast<int>(texcoords.size());
}
int ObjBuilder::countVertex() {
        allVertexCount++;
        return allVertexCount;
}
int ObjBuilder::countVertex(std::vector<int>& cache, int modelIndex) const {
        int verts = -1;
        if (modelIndex < static_cast<int>(cache.size())) {
                verts = cache.at(modelIndex);
        }
        if (verts == -1) {
                if (modelIndex == 0) {
                        verts = static_cast<int>(models.at(0)->vertices.size());
                } else {
                        verts = countVertex(cache, modelIndex - 1);
                        verts += models[modelIndex]->vertices.size();
                }
                while (static_cast<int>(cache.size()) <= modelIndex) {
                        cache.emplace_back(-1);
                }
                cache[modelIndex] = verts;
        }
        return verts;
}
int ObjBuilder::countNormal(std::vector<int>& cache, int modelIndex) const {
        int norms = -1;
        if (modelIndex < static_cast<int>(cache.size())) {
                norms = cache.at(modelIndex);
        }
        if (norms == -1) {
                if (modelIndex == 0) {
                        norms = static_cast<int>(models.at(0)->normals.size());
                } else {
                        norms = countVertex(cache, modelIndex - 1);
                        norms += models[modelIndex]->normals.size();
                }
                while (static_cast<int>(cache.size()) <= modelIndex) {
                        cache.emplace_back(-1);
                }
                cache[modelIndex] = norms;
        }
        return norms;
}
int ObjBuilder::countTexcoord(std::vector<int>& cache, int modelIndex) const {
        int texcoords = -1;
        if (modelIndex < static_cast<int>(cache.size())) {
                texcoords = cache.at(modelIndex);
        }
        if (texcoords == -1) {
                if (modelIndex == 0) {
                        texcoords =
                            static_cast<int>(models.at(0)->texcoords.size());
                } else {
                        texcoords = countVertex(cache, modelIndex - 1);
                        texcoords += models[modelIndex]->texcoords.size();
                }
                while (static_cast<int>(cache.size()) <= modelIndex) {
                        cache.emplace_back(-1);
                }
                cache[modelIndex] = texcoords;
        }
        return texcoords;
}
int ObjBuilder::resolveVertexIndex(std::vector<int>& cache, int modelIndex,
                                   int localVertexIndex) const {
        if (localVertexIndex == -1) {
                return -1;
        }
        return countVertex(cache, modelIndex) -
               static_cast<int>(models[modelIndex]->vertices.size()) +
               localVertexIndex + static_cast<int>(this->vertices.size());
}
int ObjBuilder::resolveNormalIndex(std::vector<int>& cache, int modelIndex,
                                   int localNormalIndex) const {
        if (localNormalIndex == -1) {
                return -1;
        }
        return countNormal(cache, modelIndex) -
               static_cast<int>(models[modelIndex]->normals.size()) +
               localNormalIndex + static_cast<int>(this->normals.size());
}
int ObjBuilder::resolveTexcoordIndex(std::vector<int>& cache, int modelIndex,
                                     int localTexcoordIndex) const {
        if (localTexcoordIndex == -1) {
                return -1;
        }
        return countTexcoord(cache, modelIndex) -
               static_cast<int>(models[modelIndex]->texcoords.size()) +
               localTexcoordIndex + static_cast<int>(this->texcoords.size());
}
}  // namespace objb
}  // namespace planet
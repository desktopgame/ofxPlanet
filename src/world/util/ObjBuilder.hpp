#pragma once
#ifndef WORLD_OBJB_OBJBUILDER_HPP
#define WORLD_OBJB_OBJBUILDER_HPP
#include <glm/glm.hpp>
#include <ostream>
#include <string>
#include <vector>

namespace ofxPlanet {
/**
 * IndexMode.
 */
enum class IndexMode {
        Local,
        Global,
};
/**
 * ObjIndex.
 */
struct ObjIndex {
        int index;
        bool valid;
        IndexMode mode;

        explicit ObjIndex(int index, IndexMode mode);
        explicit ObjIndex(int index);
        explicit ObjIndex();
};
/**
 * ObjPolygon is one vertex.
 */
struct ObjPolygon {
        ObjIndex vertexIndex;
        ObjIndex texcoordIndex;
        ObjIndex normalIndex;

        explicit ObjPolygon(ObjIndex vertexIndex, ObjIndex texcoordIndex,
                            ObjIndex normalIndex);
        explicit ObjPolygon(ObjIndex vertexIndex, ObjIndex texcoordIndex);
};
/**
 * ObjFace is face.
 */
using ObjFace = std::vector<ObjPolygon>;
class ObjBuilder;
/**
 * ObjModel is model.
 */
class ObjModel {
       public:
        explicit ObjModel(ObjBuilder& builder, const std::string& name);
        /**
         * @param aVertex
         * @param destPoly
         * @return
         */
        ObjModel& sharedVertex(const glm::vec3& aVertex, ObjPolygon& destPoly);

        /**
         * add vertex.
         * @param vertex
         * @return
         */
        ObjModel& vertex(const glm::vec3& vertex);

        /**
         * add normal vector.
         * @param normal
         * @return
         */
        ObjModel& normal(const glm::vec3& normal);

        /**
         * add texcoord.
         * @param texcoord
         * @return
         */
        ObjModel& texcoord(const glm::vec2& texcoord);

        /**
         * add face.
         * @param face
         * @return
         */
        ObjModel& face(const ObjFace& face);

        /**
         * @param material
         * @return
         */
        ObjModel& useMaterial(const std::string& material);

        /**
         * @return
         */
        int getUseIndexCount();
        std::string name;
        std::string material;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texcoords;
        std::vector<ObjFace> faces;

       private:
        int vertexCount;
        int useIndexCount;
        ObjBuilder& builder;
};
/**
 * ObjBuilder is helper class for generate model.
 */
class ObjBuilder {
       public:
        explicit ObjBuilder();
        ~ObjBuilder();
        /**
         *
         * @param name
         * @return
         */
        ObjModel& newModel(const std::string& name);
        /**
         * @param size
         */
        void reserveModels(int size);

        /**
         * @param _material
         * @return
         */
        ObjBuilder& material(const std::string& _material);

        /**
         * @param vertex
         * @return
         */
        ObjBuilder& globalVertex(glm::vec3 vertex);

        /**
         * @param vertex
         * @return
         */
        ObjBuilder& globalNormal(glm::vec3 normal);

        /**
         * @param vertex
         * @return
         */
        ObjBuilder& globalTexcoord(glm::vec2 texcoord);

        /**
         * @return
         */
        std::string toString() const;

        /**
         * @param stream
         */
        template <typename StreamType>
        void write(StreamType& stream) const;

        /**
         * @param index
         * @return
         */
        ObjModel& getModelAt(int index);

        /**
         * @return
         */
        int getModelCount() const;

        /**
         * @return
         */
        int getGlobalVertexCount() const;
        /**
         * @return
         */
        int getGlobalNormalCount() const;
        /**
         * @return
         */
        int getGloalTexcoordCount() const;

        /**
         * @return
         */
        int countVertex();

       private:
        template <typename StreamType>
        void writeImpl(StreamType& stream, int index,
                       std::vector<int>& vertCache, std::vector<int>& normCache,
                       std::vector<int>& texcoordCache) const;

        std::string _material;
        int countVertex(std::vector<int>& cache, int modelIndex) const;
        int countNormal(std::vector<int>& cache, int modelIndex) const;
        int countTexcoord(std::vector<int>& cache, int modelIndex) const;
        int resolveVertexIndex(std::vector<int>& cache, int modelIndex,
                               int localVertexIndex) const;
        int resolveNormalIndex(std::vector<int>& cache, int modelIndex,
                               int localNormalIndex) const;
        int resolveTexcoordIndex(std::vector<int>& cache, int modelIndex,
                                 int localTexcoordIndex) const;
        std::vector<ObjModel*> models;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texcoords;
        int allVertexCount;
};
template <typename StreamType>
inline void ObjBuilder::write(StreamType& stream) const {
        if (!_material.empty()) {
                stream << "mtllib " << _material << std::endl;
        }
        std::vector<int> vcache, ncache, tccache;
        int size = static_cast<int>(models.size());
        stream << "# Global Data" << std::endl;
        for (auto vert : vertices) {
                stream << "v " << vert.x << " " << vert.y << " " << vert.z
                       << std::endl;
        }
        for (auto norm : normals) {
                stream << "vn " << norm.x << " " << norm.y << " " << norm.z
                       << std::endl;
        }
        for (auto texcoord : texcoords) {
                stream << "vt " << texcoord.x << " " << texcoord.y << std::endl;
        }
        stream << "# Local Data" << std::endl;
        for (int i = 0; i < size; i++) {
                auto model = models.at(i);
                for (auto vert : model->vertices) {
                        stream << "v " << vert.x << " " << vert.y << " "
                               << vert.z << std::endl;
                }
                for (auto norm : model->normals) {
                        stream << "vn " << norm.x << " " << norm.y << " "
                               << norm.z << std::endl;
                }
                for (auto texcoord : model->texcoords) {
                        stream << "vt " << texcoord.x << " " << texcoord.y
                               << std::endl;
                }
        }
        for (int i = 0; i < size; i++) {
                writeImpl(stream, i, vcache, ncache, tccache);
        }
}
template <typename StreamType>
inline void ObjBuilder::writeImpl(StreamType& stream, int index,
                                  std::vector<int>& vertCache,
                                  std::vector<int>& normCache,
                                  std::vector<int>& texcoordCache) const {
        auto model = models.at(index);
        for (auto face : model->faces) {
                stream << "o " << model->name << std::endl;
                if (!model->material.empty()) {
                        stream << "usemtl " << model->material << std::endl;
                }
                stream << "f ";
                for (int i = 0; i < static_cast<int>(face.size()); i++) {
                        bool last = i == (face.size() - 1);
                        auto polygon = face[i];
                        int vi = polygon.vertexIndex.index;
                        int ti = polygon.texcoordIndex.index;
                        int ni = polygon.normalIndex.index;
                        if (polygon.vertexIndex.mode == IndexMode::Local) {
                                vi = resolveVertexIndex(vertCache, index, vi);
                        }
                        if (polygon.normalIndex.mode == IndexMode::Local) {
                                ni = resolveNormalIndex(normCache, index, ni);
                        }
                        if (polygon.texcoordIndex.mode == IndexMode::Local) {
                                ti = resolveTexcoordIndex(texcoordCache, index,
                                                          ti);
                        }
                        if (polygon.vertexIndex.valid &&
                            polygon.texcoordIndex.valid &&
                            polygon.normalIndex.valid) {
                                stream << vi << "/" << ti << "/" << ni;
                        } else if (polygon.vertexIndex.valid &&
                                   polygon.texcoordIndex.valid) {
                                stream << vi << "/" << ti;
                        } else if (polygon.vertexIndex.valid) {
                                stream << vi;
                        }
                        if (!last) {
                                stream << " ";
                        }
                }
                stream << std::endl;
        }
}
}  // namespace ofxPlanet
#endif
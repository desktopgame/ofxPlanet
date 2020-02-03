#pragma once
#ifndef WORLD_OBJB_OBJBUILDER_HPP
#define WORLD_OBJB_OBJBUILDER_HPP
#include <glm/glm.hpp>
#include <ostream>
#include <string>
#include <vector>

namespace planet {
namespace objb {
/**
 * IndexMode は、あるインデックスが現在のモデルからの位置であるか、
 * 全ての頂点からの位置であるかを指定する列挙型です。
 */
enum class IndexMode {
        Local,
        Global,
};
/**
 * ObjIndex は頂点/UV/法線を参照するための整数です。
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
 * ObjPolygon は頂点一つあたりの情報の一覧です。
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
 * ObjFace は頂点の一覧で構成される面です。
 */
using ObjFace = std::vector<ObjPolygon>;
class ObjBuilder;
/**
 * ObjModel はモデルです。
 */
class ObjModel {
       public:
        explicit ObjModel(ObjBuilder& builder, const std::string& name);
        /**
         * 頂点が既に追加されているならそれを参照するように destPoly を変更し、
         * そうでなければ新規に頂点を追加してそれを参照するように destPoly
         * を変更します。
         * @param aVertex
         * @param destPoly
         * @return
         */
        ObjModel& sharedVertex(const glm::vec3& aVertex, ObjPolygon& destPoly);

        /**
         * 頂点を追加します。
         * @param vertex
         * @return
         */
        ObjModel& vertex(const glm::vec3& vertex);

        /**
         * 法線を追加します。
         * @param normal
         * @return
         */
        ObjModel& normal(const glm::vec3& normal);

        /**
         * UV座標を追加します。
         * @param texcoord
         * @return
         */
        ObjModel& texcoord(const glm::vec2& texcoord);

        /**
         * 面を追加します。
         * @param face
         * @return
         */
        ObjModel& face(const ObjFace& face);

        /**
         * このモデルで使用するマテリアルを指定します。
         * @param material
         * @return
         */
        ObjModel& useMaterial(const std::string& material);
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
 * ObjBuilder は .obj フォーマットのモデルを出力するためのヘルパーです。
 */
class ObjBuilder {
       public:
        explicit ObjBuilder();
        ~ObjBuilder();
        /**
         * 新しいモデルを生成します。
         * @param name
         * @return
         */
        ObjModel& newModel(const std::string& name);
        /**
         * 事前に必要になるモデルの数がおおよそ分かっているなら、
         * 事前にベクトルを伸長させます。
         * @param size
         */
        void reserveModels(int size);

        /**
         * 読み込むマテリアル定義ファイルを追加します。
         * @param _material
         * @return
         */
        ObjBuilder& material(const std::string& _material);

        /**
         * 特定のモデルに紐付けられないグローバルな頂点を追加します。
         * @param vertex
         * @return
         */
        ObjBuilder& globalVertex(glm::vec3 vertex);

        /**
         * 特定のモデルに紐付けられないグローバルな法線を追加します。
         * @param vertex
         * @return
         */
        ObjBuilder& globalNormal(glm::vec3 normal);

        /**
         * 特定のモデルに紐付けられないグローバルなUVを追加します。
         * @param vertex
         * @return
         */
        ObjBuilder& globalTexcoord(glm::vec2 texcoord);

        /**
         * 現在の obj定義 を文字列で返します。
         * @return
         */
        std::string toString() const;

        /**
         * 現在の obj定義 をストリームへ直接書き込みます。
         * @param stream
         */
        template <typename StreamType>
        void write(StreamType& stream) const;

        /**
         * 指定位置のモデルを返します。
         * @param index
         * @return
         */
        ObjModel& getModelAt(int index);

        /**
         * 全てのモデルの数を返します。
         * @return
         */
        int getModelCount() const;

        /**
         * グローバルな頂点の数を返します。
         * @return
         */
        int getGlobalVertexCount() const;
        /**
         * グローバルな法線の数を返します。
         * @return
         */
        int getGlobalNormalCount() const;
        /**
         * グローバルなUVの数を返します。
         * @return
         */
        int getGloalTexcoordCount() const;

        /**
         * 頂点をカウントして返します。
         * 最初は 1 を返します。
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
        // 先に頂点, 法線, UVだけを書き込んでおく
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
        // 面を作成する
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
}  // namespace objb
}  // namespace planet
#endif
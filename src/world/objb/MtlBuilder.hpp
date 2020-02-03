#pragma once
#ifndef WORLD_OBJB_MTLBUILDER_HPP
#define WORLD_OBJB_MTLBUILDER_HPP
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace planet {
namespace objb {
/**
 * MtlElement は、マテリアル定義です。
 */
class MtlElement {
       public:
        explicit MtlElement(const std::string& name);
        MtlElement& ns(float _ns);
        MtlElement& ka(glm::vec3 _ka);
        MtlElement& kd(glm::vec3 _kd);
        MtlElement& ks(glm::vec3 _ks);
        MtlElement& ke(glm::vec3 _ke);
        MtlElement& ni(float _ni);
        MtlElement& d(float _d);
        MtlElement& illum(float _illum);
        MtlElement& map_Kd(const std::string& _map_Kd);

        std::string name;
        float _ns;
        glm::vec3 _ka;
        glm::vec3 _kd;
        glm::vec3 _ks;
        glm::vec3 _ke;
        float _ni;
        float _d;
        float _illum;
        std::string _map_Kd;
};
/**
 * MtlBuilder は .mtl フォーマットのモデルを出力するためのヘルパーです。
 */
class MtlBuilder {
       public:
        explicit MtlBuilder();
        ~MtlBuilder();
        MtlElement& newElement(const std::string& name);
        std::string toString() const;

       private:
        std::vector<MtlElement*> elements;
};
}  // namespace objb
}  // namespace planet
#endif
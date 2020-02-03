#include "MtlBuilder.hpp"

#include <sstream>

namespace planet {
namespace objb {
// MtlElement
MtlElement::MtlElement(const std::string& name)
    : name(name),
      _ns(225),
      _ka(1, 1, 1),
      _kd(0.8f, 0.8f, 0.8f),
      _ks(0.5f, 0.5f, 0.5f),
      _ke(0, 0, 0),
      _ni(1.45f),
      _d(1) {}
MtlElement& MtlElement::ns(float _ns) {
        this->_ns = _ns;
        return *this;
}
MtlElement& MtlElement::ka(glm::vec3 _ka) {
        this->_ka = _ka;
        return *this;
}
MtlElement& MtlElement::kd(glm::vec3 _kd) {
        this->_kd = _kd;
        return *this;
}
MtlElement& MtlElement::ks(glm::vec3 _ks) {
        this->_ks = _ks;
        return *this;
}
MtlElement& MtlElement::ke(glm::vec3 _ke) {
        this->_ke = _ke;
        return *this;
}
MtlElement& MtlElement::ni(float _ni) {
        this->_ni = _ni;
        return *this;
}
MtlElement& MtlElement::d(float _d) {
        this->_d = _d;
        return *this;
}
MtlElement& MtlElement::illum(float _illum) {
        this->_illum = _illum;
        return *this;
}
MtlElement& MtlElement::map_Kd(const std::string& _map_Kd) {
        this->_map_Kd = _map_Kd;
        return *this;
}
// MtlBuilder
MtlBuilder::MtlBuilder() : elements() {}
MtlBuilder::~MtlBuilder() {
        for (auto e : elements) {
                delete e;
        }
        elements.clear();
}
MtlElement& MtlBuilder::newElement(const std::string& name) {
        auto elem = new MtlElement(name);
        elements.emplace_back(elem);
        return *elem;
}
std::string MtlBuilder::toString() const {
        std::stringstream ss;
        for (auto e : elements) {
                ss << "newmtl " << e->name << std::endl;
                ss << "Ns " << e->_ns << std::endl;
                ss << "Ka " << e->_ka.x << " " << e->_ka.y << " " << e->_ka.z
                   << std::endl;
                ss << "Kd " << e->_kd.x << " " << e->_kd.y << " " << e->_kd.z
                   << std::endl;
                ss << "Ks " << e->_ks.x << " " << e->_ks.y << " " << e->_ks.z
                   << std::endl;
                ss << "Ke " << e->_ke.x << " " << e->_ke.y << " " << e->_ke.z
                   << std::endl;
                ss << "Ni " << e->_ni << std::endl;
                ss << "d " << e->_d << std::endl;
                ss << "illum " << e->_illum << std::endl;
                ss << "map_Kd " << e->_map_Kd << std::endl;
        }
        ss << std::endl;
        return ss.str();
}
}  // namespace objb
}  // namespace planet
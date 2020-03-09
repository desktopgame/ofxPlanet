#ifndef WORLD_UNIFORMLAYOUT_HPP
#define WORLD_UNIFORMLAYOUT_HPP
#include <string>

namespace ofxPlanet {
class UniformLayout {
       public:
        static std::string BRIGHTNESSS_NAME;
		static std::string MODELMATRIX_NAME;

       private:
        UniformLayout() = delete;
        ~UniformLayout() = delete;
};
}  // namespace ofxPlanet
#endif
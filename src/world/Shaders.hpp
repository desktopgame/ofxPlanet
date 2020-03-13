#ifndef WORLD_SHADERS_HPP
#define WORLD_SHADERS_HPP
#include <string>

namespace ofxPlanet {
class Shaders {
public:
	static const std::string STANDARD_VERTEX_SHADER;
	static const std::string STANDARD_FRAGMENT_SHADER;
	static const std::string STANDARD_UNIFORM_VIEWMATRIX_NAME;
	static const std::string STANDARD_UNIFORM_PROJECTIONMATRIX_NAME;
private:
	Shaders() = delete;
	~Shaders() = delete;
};
}
#endif
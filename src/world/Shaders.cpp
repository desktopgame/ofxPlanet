#include "Shaders.hpp"
namespace ofxPlanet {
// Standard Shader
const std::string Shaders::STANDARD_VERTEX_SHADER = R"(
#version 410
layout(location=0) in vec3 aVertex;
layout(location=3) in vec2 aUV;
layout(location=4) in ivec3 aPosition;
uniform mat4 uModelMatrix;

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
out vec4 position;
out vec2 uv;
void main(void) {
  position = (uProjectionMatrix * uViewMatrix * uModelMatrix) * vec4(aVertex+aPosition, 1);
  uv = aUV;
  gl_Position = position;
})";

const std::string Shaders::STANDARD_FRAGMENT_SHADER = R"(
#version 410
out vec4 fragColor;
uniform sampler2D uTexture;
uniform float uBrightness;
in vec2 uv;

void main (void) {
  vec3 black = vec3(0, 0, 0);
  vec4 diffuse = texture(uTexture, uv);
  vec3 color = mix(diffuse.rgb, black, uBrightness);
  fragColor = vec4(color.rgb, diffuse.a);
}
)";
const std::string Shaders::STANDARD_UNIFORM_VIEWMATRIX_NAME = "uViewMatrix";
const std::string Shaders::STANDARD_UNIFORM_PROJECTIONMATRIX_NAME =
    "uProjectionMatrix";
}  // namespace ofxPlanet
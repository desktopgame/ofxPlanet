# Vertex Layout
this page is document for customize shader on ofxPlanet.

## Attribute Variables
### Vertex
Vertex is vertex for shape. attribute index is zero, type is "vec3" in GLSL.  
this is using openFrameworks default bindings.

### TexCoord
TexCoord is uv position on texture. attribute index is three, type is "vec2" in GLSL.  
this is using openFrameworks default bindings.

### Position
Position is position for shape. attribute is four, type is "ivec3" in GLSL.  
reason of use this variable is ofxPlanet is using "instanced draw".

## Uniform Variables
### Brightness
Brightness is using for lighting.  
should be this variable is defined "uBrightness", type is "float" in GLSL.
### ModelMatrix
ModelMatrix is using for saving memory.  
should be this variable is defined "uModelMatrix", type is "mat4" in GLSL.


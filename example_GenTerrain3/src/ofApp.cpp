#include "ofApp.h"
#include "ofxPlanet.h"

ofApp::ofApp()
    : world(nullptr), fpsCon(), biome(nullptr), shader() {}

//--------------------------------------------------------------
void ofApp::setup() {
		// load shader
        shader.setupShaderFromSource(GL_VERTEX_SHADER,
                                     R"(
#version 410
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require
/*
0: position
1: color
2: normal
3: texcoord
*/
layout(location=0) in vec3 aVertex;
layout(location=3) in vec2 aUV;
layout(location=4) in vec3 aPosition;
uniform mat4 uMVPMatrix;
out vec4 position;
out vec2 uv;
void main(void) {
  position = uMVPMatrix * vec4(aVertex+aPosition, 1);
  uv = aUV;
  gl_Position = position;
})");
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER,
                                     R"(
#version 410
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require
out vec4 outputC;
uniform sampler2D uTexture;
uniform float uBrightness;
in vec2 uv;
void main (void) {
  vec3 black = vec3(0, 0, 0);
  vec4 diffuse = texture(uTexture, uv);
  vec3 color = mix(diffuse.rgb, black, uBrightness);
  outputC = vec4(color.rgb, diffuse.a);
}
)");
        shader.bindDefaults();
        shader.linkProgram();
		// load block define files.
        auto texBuf = ofBufferFromFile("textures.json");
        auto blockBuf = ofBufferFromFile("blocks.json");
        ofxPlanet::TextureInfoCollection tic;
        tic.deserialize(std::string(texBuf.getData()));
        ofxPlanet::BlockInfoCollection bic;
        bic.deserialize(std::string(blockBuf.getData()));
        ofxPlanet::BlockPack::load(bic)->select();
        ofxPlanet::TexturePack::load(tic)->select();
        ofxPlanet::TexturePack::getCurrent()->resolve();
		// generate world
        this->world = ofxPlanet::World::create(shader, 64);
        this->biome = std::make_shared<MyBiome>();
		world->setBiome(biome);
		int tmp = fpsCon.upKey.keycode;
		fpsCon.upKey.keycode = fpsCon.downKey.keycode;
		fpsCon.downKey.keycode = tmp;
		fpsCon.moveSpeed = 2;
		fpsCon.transform.position = glm::vec3(64, 128, 64);
		fpsCon.rotationAxis.y = 1;
		ofxFirstPersonController::lockMouseCursor();
		ofSetOrientation(OF_ORIENTATION_DEFAULT, false);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
		shader.begin();
		shader.setUniformMatrix4f("uMVPMatrix", fpsCon.projectionMatrix() * fpsCon.viewMatrix());
		shader.end();
		world->setViewPosition(fpsCon.transform.position / 2);
		this->world->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}

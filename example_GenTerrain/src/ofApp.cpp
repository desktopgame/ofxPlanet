#include "ofApp.h"
#include "ofxPlanet.h"


ofApp::ofApp()
 : planet(nullptr), biome(nullptr),shader(), camera() {
}

//--------------------------------------------------------------
void ofApp::setup() {
	//シェーダー読み込み
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
layout(location=2) in vec3 aNormal;
layout(location=3) in vec2 aUV;
layout(location=4) in vec3 aPosition;

uniform mat4 uMVPMatrix;
uniform mat4 uNormalMatrix;
//uniform vec4 uLightPos;

out vec4 position;
out vec4 normal;
out vec2 uv;
out vec4 lightPos;
//flat out int InstanceID; 

void main(void) {
  position = uMVPMatrix * vec4(aVertex+aPosition, 1);
  lightPos = uMVPMatrix * vec4(64,32,64, 1);
  normal = normalize(uNormalMatrix * vec4(aNormal, 0));
  uv = aUV;
  gl_Position = position;
}
)");
	shader.setupShaderFromSource(GL_FRAGMENT_SHADER,
		R"(

#version 410
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

out vec4 outputC;

uniform sampler2D uTexture;
uniform float uShininess;
uniform vec4 uDiffuse;
uniform vec4 uSpecular;
uniform vec4 uAmbient;

in vec4 position;
in vec4 normal;
in vec2 uv;
in vec4 lightPos;

void main (void) {
  vec4 color = texture(uTexture, uv);
  vec3 light = normalize((lightPos * position.w - lightPos.w * position).xyz);
  vec3 fnormal = normalize(normal.xyz);
  float diffuse = max(dot(light, fnormal), 0.0);
  vec3 view = -normalize(position.xyz);
  vec3 halfway = normalize(light + view);
  float specular = pow(max(dot(fnormal, halfway), 0.0), uShininess);
  outputC = color * uDiffuse * diffuse
                + uSpecular * specular
                + color * uAmbient;
}
)");
	shader.bindDefaults();
	shader.linkProgram();
	// ブロック定義ファイルの読み込み
	auto texBuf = ofBufferFromFile("textures.json");
	auto blockBuf = ofBufferFromFile("blocks.json");
	ofxPlanet::TextureInfoCollection tic;
	tic.deserialize(std::string(texBuf.getData()));
	ofxPlanet::BlockInfoCollection bic;
	bic.deserialize(std::string(blockBuf.getData()));
	ofxPlanet::BlockPack::load(bic)->select();
	ofxPlanet::TexturePack::load(tic)->select();
	ofxPlanet::TexturePack::getCurrent()->resolve();
	//ワールド生成
	this->planet = std::make_shared<ofxPlanet::Planet>(shader);
	this->biome = std::make_shared<MyBiome>();
	planet->generate(glm::vec3(128, 64, 128), biome);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	// カメラを更新
	auto w = planet->getWorld();
	const int wsx = w->getXSize();
	const int wsy = w->getYSize();
	const int wsz = w->getZSize();
	const float fwsx = static_cast<float>(w->getXSize());
	const float fwsy = static_cast<float>(w->getYSize());
	const float fwsz = static_cast<float>(w->getZSize());
	const float hfwsx = fwsx / 2;
	const float hfwsz = fwsz / 2;
	auto cx = std::cos(cameraAngle);
	auto cz = std::sin(cameraAngle);
	camera.setScreenSize(glm::vec2(ofGetWidth(), ofGetHeight()));
	camera.setPosition(glm::vec3(hfwsx + (hfwsx * cx), wsy, hfwsz + (hfwsz * cz)) * 2);
	camera.setLookAt(glm::vec3(wsx / 2, 0, wsz / 2) * 2);
	camera.rehash();
	this->cameraAngle += 0.01f;
	// シェーダーを更新
	camera.rehash();
	shader.begin();
	shader.setUniform4f("uAmbient", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	shader.setUniform4f("uDiffuse", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	shader.setUniform4f("uSpecular", glm::vec4(0.f, 0.f, 0.f, 1.0f));
	shader.setUniform1f("uShininess", 50);
	shader.setUniformMatrix4f("uMVPMatrix", (camera.getProjectionMatrix() * camera.getViewMatrix()));
	shader.setUniformMatrix4f("uNormalMatrix", (camera.computeNormalMatrix(glm::mat4(1.0f))));
	shader.end();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	planet->drawToBuffer();
	planet->render();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

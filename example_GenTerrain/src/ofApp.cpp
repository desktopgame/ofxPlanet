#include "ofApp.h"
#include "ofxPlanet.h"

ofApp::ofApp()
    : planet(nullptr), biome(nullptr), shader(), camera(), cameraAngle() {}

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
void ofApp::update() {}

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
        camera.setPosition(
            glm::vec3(hfwsx + (hfwsx * cx), wsy, hfwsz + (hfwsz * cz)) * 2);
        camera.setLookAt(glm::vec3(wsx / 2, 0, wsz / 2) * 2);
        camera.rehash();
        this->cameraAngle += 0.01f;
        // シェーダーを更新
        camera.rehash();
        shader.begin();
        shader.setUniformMatrix4f("uMVPMatrix", (camera.getProjectionMatrix() *
                                                 camera.getViewMatrix()));
        shader.end();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        planet->drawToBuffer();
        planet->render();
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

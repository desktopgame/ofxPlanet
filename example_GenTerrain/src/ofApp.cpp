#include "ofApp.h"
#include "ofxPlanet.h"

ofApp::ofApp()
    : world(nullptr), biome(nullptr), shader(), camera(), cameraAngle() {}

//--------------------------------------------------------------
void ofApp::setup() {
		// load shader
        shader.setupShaderFromSource(GL_VERTEX_SHADER, ofxPlanet::Shaders::STANDARD_VERTEX_SHADER);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, ofxPlanet::Shaders::STANDARD_FRAGMENT_SHADER);
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
        this->world = ofxPlanet::FixedWorld::create(shader, glm::ivec3(128,64,128));
        this->biome = std::make_shared<MyBiome>();
		ofxPlanet::BlockTable bt(128, 64, 128);
		biome->generate(bt);
		world->load(bt);
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
		// update camera
		auto w = this->world;
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
        camera.setPosition(glm::vec3((cx * 64.0f)-64.0f,128,(cz*64.0f) - 64.0f));
        camera.setLookAt(glm::vec3(wsx / 2, 0, wsz / 2) * 2);
        camera.rehash();

        this->cameraAngle += 0.01f;
        // シェーダーを更新
        camera.rehash();
        shader.begin();
        shader.setUniformMatrix4f(ofxPlanet::Shaders::STANDARD_UNIFORM_VIEWMATRIX_NAME,camera.getViewMatrix());
		shader.setUniformMatrix4f(ofxPlanet::Shaders::STANDARD_UNIFORM_PROJECTIONMATRIX_NAME, camera.getProjectionMatrix());
        shader.end();

		ofSetOrientation(OF_ORIENTATION_DEFAULT, false);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		this->world->getChunk()->draw();
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

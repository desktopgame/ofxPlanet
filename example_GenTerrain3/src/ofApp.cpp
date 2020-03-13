#include "ofApp.h"
#include "ofxPlanet.h"

ofApp::ofApp()
    : world(nullptr), fpsCon(), biome(nullptr), shader() {}

//--------------------------------------------------------------
void ofApp::setup() {
		// load shader
        shader.setupShaderFromSource(GL_VERTEX_SHADER,ofxPlanet::Shaders::STANDARD_VERTEX_SHADER);
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER,ofxPlanet::Shaders::STANDARD_FRAGMENT_SHADER);
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
		shader.setUniformMatrix4f(ofxPlanet::Shaders::STANDARD_UNIFORM_VIEWMATRIX_NAME, fpsCon.viewMatrix());
		shader.setUniformMatrix4f(ofxPlanet::Shaders::STANDARD_UNIFORM_PROJECTIONMATRIX_NAME, fpsCon.projectionMatrix());
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

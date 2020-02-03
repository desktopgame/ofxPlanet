#pragma once
#include <ofMain.h>
#include <ofShader.h>
#include "Camera.hpp"
#include "MyBiome.hpp"

namespace ofxPlanet {
	class Planet;
	class BasicBiome;
}
class ofApp : public ofBaseApp {

public:
	ofApp();
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
private:
	ofShader shader;
	Camera camera;
	float cameraAngle;
	std::shared_ptr<ofxPlanet::Planet> planet;
	std::shared_ptr<MyBiome> biome;
};

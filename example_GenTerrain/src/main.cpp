#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings settings;
	settings.glVersionMajor = 4;
	settings.glVersionMinor = 1;
	settings.setSize(800, 600);
	settings.windowMode = ofWindowMode::OF_WINDOW;
	settings.title = "ofxPlanet";
#ifdef _DEBUG
	ofSetLogLevel(ofLogLevel::OF_LOG_FATAL_ERROR);
#endif
	ofCreateWindow(settings);
	ofRunApp(new ofApp());

}

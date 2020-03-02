#include "ofApp.h"
#include "ofMain.h"

//========================================================================
int main() {
        ofGLFWWindowSettings settings;
        settings.glVersionMajor = 4;
        settings.glVersionMinor = 5;
        settings.setSize(1280, 720);
        settings.windowMode = ofWindowMode::OF_WINDOW;
        settings.title = "ofxPlanet";
#ifdef _DEBUG
        ofSetLogLevel(ofLogLevel::OF_LOG_FATAL_ERROR);
#endif
        ofCreateWindow(settings);
        ofRunApp(new ofApp());
}

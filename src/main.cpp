#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
//	ofSetupOpenGL(1080/2,1920/2, OF_WINDOW);			// <-------- setup the GL context
    ofSetupOpenGL(1000,1000, OF_WINDOW);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}

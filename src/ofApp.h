#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
    
        void createMesh();
        void updateMesh();
        void processOpenFileSelection(ofFileDialogResult openFileResult);
        
        void onButtonEvent(ofxDatGuiButtonEvent e);
        void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
        void onSliderEvent(ofxDatGuiSliderEvent e);
        
    
    string filename;
    
    ofImage img;
    ofMesh mesh;
    ofEasyCam easyCam;
    
    int numVerts;
    
    
    
    //save old
    bool record = false;
    int counter = 0;
    //-----------------
    
    //Save big
    ofFbo largeOffscreenImage;
    int ofscreenW, ofscreenH;
    //-------------------------
    
    // darstelltung
    int pointSize;
    
    
    ofxDatGui* gui = new ofxDatGui(0, 0);
    ofxDatGuiButton* buttonOpen;
    ofxDatGuiColorPicker* colorPickBg;
    ofxDatGuiSlider* depthSlider;
    
    ofColor bgColor;
    int depthSliderVal = 800;
    
    
    
    
		
};

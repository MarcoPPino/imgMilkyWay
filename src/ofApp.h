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
        void onDropdownEvent(ofxDatGuiDropdownEvent e);
        void onSliderEvent(ofxDatGuiSliderEvent e);
        void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
        
    
    string filename;
    
    ofImage img;
    ofMesh mesh;
    ofEasyCam easyCam;
    
    int numVerts;
    int dropdownVal = 2;
    
    
    
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
    
    ofxDatGuiDropdown* elevateDropDown;
    ofxDatGuiSlider* depthSlider;
    ofxDatGuiColorPicker* colorPickBg;
    
    ofxDatGuiFolder* infoFolder;
    ofxDatGuiLabel* labelImg;
    ofxDatGuiLabel* labelImgSize;
    ofxDatGuiLabel* labelZ;
    ofxDatGuiLabel* labelNum;
    
    ofColor bgColor;
    int depthSliderVal = 800;
    
    
    
    
		
};

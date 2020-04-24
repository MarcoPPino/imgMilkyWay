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
        void saveImg();
        
        void onButtonEvent(ofxDatGuiButtonEvent e);
        void onDropdownEvent(ofxDatGuiDropdownEvent e);
        void onSliderEvent(ofxDatGuiSliderEvent e);
        void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    
    
    ofxDatGui* gui = new ofxDatGui(0, 0);
    
    ofxDatGuiButton* buttonOpen;
    
    ofxDatGuiDropdown* elevateDropDown;
    ofxDatGuiDropdown* modeDropdown;
    ofxDatGuiSlider* depthSlider;
    ofxDatGuiSlider* pointSlider;
    ofxDatGuiColorPicker* colorPickBg;
    
    ofxDatGuiFolder* infoFolder;
    ofxDatGuiLabel* labelImg;
    ofxDatGuiLabel* labelImgSize;
    ofxDatGuiLabel* labelZ;
    ofxDatGuiLabel* labelNum;
    
    ofxDatGuiFolder* saveFolder;
    ofxDatGuiTextInput* inputWidth;
    ofxDatGuiTextInput* inputHeight;
    ofxDatGuiToggle* toggleJpg;
    ofxDatGuiToggle* togglePng;
    ofxDatGuiToggle* toggleSpace;
    ofxDatGuiButton* buttonSave;
    
    vector<string> ElevateOptions = {"Brightness", "Lightness", "Saturation"};
    vector<string> modeOptions = {"Points", "Triangles", "Triangle Strip", "Triangle Fan - Fucking Slow!", "Lines", "Line Strip", "Line Loop"};
        
    
    
    
    
    string filename;
    
    ofImage img;
    ofMesh mesh;
    ofEasyCam easyCam;
    
    int numVerts;
    int dropdownVal = 2;
    string modeDropdownVal = "OF_PRIMITIVE_TRIANGLE_STRIP";
    
    
    
    //save old
    bool record = false;
    int counter = 0;
    //-----------------
    
    //Save big
    ofFbo largeOffscreenImage;
    int ofscreenW, ofscreenH;
    //-------------------------
    
    // darstelltung
    int pointSliderVal = 1;
    

    ofColor bgColor;
    int depthSliderVal = 400;
    
    
    
    
		
};

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
        //------------------GUI stuff-------------------------
        void onButtonEvent(ofxDatGuiButtonEvent e);
        void onDropdownEvent(ofxDatGuiDropdownEvent e);
        void onSliderEvent(ofxDatGuiSliderEvent e);
        void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
        void onToggleEvent(ofxDatGuiToggleEvent e);
        void onTextInputEvent(ofxDatGuiTextInputEvent e);
    
    
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
    ofxDatGuiDropdown* sizePicker;
    ofxDatGuiToggle* toggleJpg;
    ofxDatGuiToggle* togglePng;
    ofxDatGuiToggle* toggleSpace;
    ofxDatGuiToggle* toggleAA;
    ofxDatGuiButton* buttonSave;
    
    int pointSliderVal = 1;
    int depthSliderVal = 400;
    int dropdownVal = 2;
    vector<string> ElevateOptions = {"Brightness", "Lightness", "Saturation"};
    vector<string> modeOptions = {"Points", "Triangles", "Triangle Strip", "Triangle Fan - Fucking Slow!", "Lines", "Line Strip", "Line Loop"};
    
    vector<string> SizeOptions = {"DIN A1" ,"DIN A2", "DIN A3", "DIN A4", "DIN A5", "DIN A6 (Postcard)", "10x10cm", "20x20cm", "30x30cm", "Instagram Post Square", "Instagram Post Portrait", "Instagram Post Landscape", "Instagram Stories"};

    
    //------------------Viz-------------------------
    ofImage img;
    ofMesh mesh;
    ofEasyCam easyCam;
    int numVerts;
    ofColor bgColor;
    

    //------------------saving-------------------------
    string filename;
    ofFbo largeOffscreenImage;
    int counter = 0;
    bool record = false;
		
};

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    
    filename = "t4.jpg";
    img.load(filename);
    
    mesh.setMode(OF_PRIMITIVE_POINTS);
    mesh.enableIndices();
    mesh.enableColors();
    
    ofDisableAntiAliasing();
    
    
    
    createMesh();
    
    
    
    //GUI------------------------------------------------------
    
    buttonOpen = gui->addButton("Load Image");
    gui->addBreak()->setHeight(15.0f);
    elevateDropDown = gui->addDropdown("Elevation mode: " , ElevateOptions);
    modeDropdown = gui->addDropdown("Point Connection mode: " , modeOptions);
    gui->addBreak()->setHeight(10.0f);
    depthSlider = gui->addSlider("Depth", 0, 1000, depthSliderVal);
    pointSlider = gui->addSlider("Pointsize", 0, 10, pointSliderVal);
    colorPickBg = gui->addColorPicker("BackgroundColor", ofColor::fromHex(0x000000));
    gui->addBreak()->setHeight(10.0f);
    
    
    infoFolder = gui->addFolder("Infos", ofColor::white);
    labelImg = infoFolder->addLabel("Filename: " + ofToString(filename));
    labelImgSize = infoFolder->addLabel("ImageSize: " + ofToString(img.getWidth()) + " x " + ofToString(img.getHeight()));
    labelNum = infoFolder->addLabel("Number of Points: " + ofToString(numVerts));
    labelZ = infoFolder->addLabel("Zoom: ");
    infoFolder->addFRM();
    gui->addBreak()->setHeight(15.0f);
    
    saveFolder = gui->addFolder("Save Options", ofColor::white);
    inputWidth = saveFolder->addTextInput("Width", "1920");
    inputHeight = saveFolder->addTextInput("Height", "1080");
    toggleJpg = saveFolder->addToggle("Save as JPEG");
    toggleJpg->setChecked(true);
    togglePng = saveFolder->addToggle("Save as PNG");
    toggleSpace = saveFolder->addToggle("Use Spacebar to save");
    toggleAA = saveFolder->addToggle("Anti Aliasing");
    buttonSave = gui->addButton("Save Image");
    
    buttonOpen->onButtonEvent(this, &ofApp::onButtonEvent);
    buttonSave->onButtonEvent(this, &ofApp::onButtonEvent);
    elevateDropDown->onDropdownEvent(this, &ofApp::onDropdownEvent);
    modeDropdown->onDropdownEvent(this, &ofApp::onDropdownEvent);
    colorPickBg->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    depthSlider->onSliderEvent(this, &ofApp::onSliderEvent);
    pointSlider->onSliderEvent(this, &ofApp::onSliderEvent);
    inputWidth->onTextInputEvent(this, &ofApp::onTextInputEvent);
    inputHeight->onTextInputEvent(this, &ofApp::onTextInputEvent);
    toggleAA->onToggleEvent(this, &ofApp::onToggleEvent);
    
    bgColor = ofColor(0,0,0);
}


//--------------------------------------------------------------
void ofApp::update(){
    labelZ->setLabel("Current Zoom: " + ofToString(easyCam.getDistance()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(bgColor);
    if(record){
        saveImg();
    }else{
        easyCam.begin();
//        cout << "Position: " << ofToString(easyCam.getPosition()) << "\n" << endl;
//        cout << "Orientation: " << ofToString(easyCam.getOrientationQuat()) << "\n" << endl;
        glPointSize(pointSliderVal);
            ofPushMatrix();
                ofTranslate(-img.getWidth()/2,-img.getHeight()/2);
                mesh.draw();
            ofPopMatrix();
        easyCam.end();
    }
    gui->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == ' '){
        if(toggleSpace->getChecked()){
            saveImg();
        }
    }
    
}

//---------------------------CREATE MESH-----------------------------------
void ofApp::createMesh(){
    mesh.clear();
    int w = img.getWidth();
    int h = img.getHeight();

    for(int x = 0; x < w; x++){
        for(int y = 0; y < h; y++){
            ofColor clr = img.getColor(x, y);
            float saturation = clr.getBrightness();
            float z = ofMap(saturation, 0, 255, 0, depthSliderVal);
            ofVec3f pos(x, y, z);
            mesh.addVertex(pos);
            mesh.addColor(clr);
        }
    }
    numVerts = mesh.getNumVertices();
}

//------------------------UPDATE MESH--------------------------------------
void ofApp::updateMesh(){
    float elevateValue;
    for (int i=0; i<numVerts; ++i) {
        ofVec3f vert = mesh.getVertex(i);
        ofColor clr = mesh.getColor(i);
        
        if(dropdownVal == 0){
            elevateValue = clr.getBrightness();
        }
        if(dropdownVal == 1){
            elevateValue = clr.getLightness();
        }
        if(dropdownVal == 2){
            elevateValue = clr.getSaturation();
        }
        float z = ofMap(elevateValue, 0, 255, 0, depthSliderVal);

        vert.x = vert.x;
        vert.y = vert.y;
        vert.z = z;
        mesh.setVertex(i, vert);
    }
}

//------------------------FILE SELECTION--------------------------------------
void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult){
    ofLogVerbose("getName(): "  + openFileResult.getName());
    ofLogVerbose("getPath(): "  + openFileResult.getPath());
    
    ofFile file (openFileResult.getPath());
    
    if (file.exists()){
    
    ofLogVerbose("The file exists - now checking the type via file extension");
    string fileExtension = ofToUpper(file.getExtension());
    
        if (fileExtension == "PNG" || fileExtension == "JPG" || fileExtension == "JPEG") {
            ofLogVerbose("File could be loaded, it's a Picture!");
            filename = openFileResult.getName();
            img.load(openFileResult.getPath());
            createMesh();
        }
    }
    
    labelImg->setLabel("Filename: " + ofToString(filename));
    labelImgSize->setLabel("ImageSize: " + ofToString(img.getWidth()) + " x " + ofToString(img.getHeight()));
    labelNum->setLabel("Number of Points: " + ofToString(numVerts));
    
}

//------------------------SaveImg--------------------------------------
void ofApp::saveImg(){
    int saveWidth = ofToInt(inputWidth->getText());
    int saveHeight = ofToInt(inputHeight->getText());
    largeOffscreenImage.allocate(saveWidth, saveHeight, GL_RGBA);
    
    int pointMultiply;
    if(saveHeight > ofGetHeight()-1){ //-1 is a hack. do some searches
        pointMultiply = saveHeight/1000;
        cout<< ofToString(pointMultiply) << "\n" << endl;
    }
    
        
        string saveFile = filename + "_" + "Zoom_" + ofToString(easyCam.getDistance()) + "_" + "Point_" + ofToString(pointSliderVal);
    
        largeOffscreenImage.begin();
//            ofClear(bgColor);
            easyCam.begin();
                glPointSize(pointSliderVal * pointMultiply); // needs work: point size to export size
                ofPushMatrix();
                ofTranslate(-img.getWidth()/2,-img.getHeight()/2);
                mesh.draw();
                ofPopMatrix();
            easyCam.end();
        largeOffscreenImage.end();
        
        ofPixels p;
        largeOffscreenImage.readToPixels(p);
    
        if(toggleJpg->getChecked()){
            ofSaveImage(p, "exports/testexports/" + saveFile + ".jpg");
        }
        if(togglePng->getChecked()){
            ofSaveImage(p, "exports/" + saveFile + ".png");
        }
        cout << "saved file: " << saveFile <<"\n" << endl;
    
        record = false;
}

//----------------------UI EVENTS----------------------------------------
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e){
    if(e.target == buttonOpen){
        ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a picture");
        if (openFileResult.bSuccess){
            ofLogVerbose("User selected a file");
            processOpenFileSelection(openFileResult);
        }else {
            ofLogVerbose("User hit cancel");
        }
    }
    if(e.target == buttonSave){
        record = true;
    }
}

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e){
    if(e.target == elevateDropDown){
        dropdownVal =  e.child;
        cout << dropdownVal << "\n" << endl;
        updateMesh();
    }
    if(e.target == modeDropdown){
        if(e.child == 0){
            mesh.setMode(OF_PRIMITIVE_POINTS);
        }
        if(e.child == 1){
            mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        }
        if(e.child == 2){
            mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        }
        if(e.child == 3){
            mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        }
        if(e.child == 4){
            mesh.setMode(OF_PRIMITIVE_LINES);
        }
        if(e.child == 5){
            mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
        }
        if(e.child == 6){
            mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
        }
    }
}

vector<string> modeOptions = {"Points","Triangles", "Triangle Strip", "Triangle Fan", "Lines", "Line Strip", "Line Loop"};

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e){
    if(e.target == depthSlider){
    depthSliderVal = depthSlider->getValue();
    updateMesh();
    }
    if(e.target == pointSlider){
        pointSliderVal = pointSlider->getValue();
    }
}

void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e){
    bgColor = e.color;
}

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e){
    if(!e.checked){
        ofDisableAntiAliasing();
    }else{
        ofEnableAntiAliasing();
    }
}

void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e){
    if(e.target == inputWidth){
        if(ofToInt(e.text) < ofGetWidth()){
            inputWidth->setText(ofToString(ofGetWidth()));
        }
    }
    if(e.target == inputHeight){
        if(ofToInt(e.text) < ofGetHeight()){
            inputHeight->setText(ofToString(ofGetHeight()));
        }
    }
}





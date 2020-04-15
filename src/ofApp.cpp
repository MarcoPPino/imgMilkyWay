#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    
    filename = "t1_2.jpg";
    img.load(filename);
    
    mesh.setMode(OF_PRIMITIVE_POINTS);
    mesh.enableIndices();
    mesh.enableColors();
    
    createMesh();
    
    //SaveFBO stuff -------------------------------------------
    ofscreenW = 5000;
    ofscreenH = 5000;
    largeOffscreenImage.allocate(ofscreenW, ofscreenH, GL_RGBA);

    //GUI------------------------------------------------------
    
    gui->setTheme(new ofxDatGuiThemeSmoke());
    buttonOpen = gui->addButton("Load Image");
    gui->addBreak()->setHeight(20.0f);
    elevateDropDown = gui->addDropdown("Elevation mode: " , ElevateOptions);
    modeDropdown = gui->addDropdown("Point Connection mode: " , modeOptions);
    depthSlider = gui->addSlider("Depth", 0, 1000, depthSliderVal);
    pointSlider = gui->addSlider("Pointsize", 0, 10, pointSliderVal);
    colorPickBg = gui->addColorPicker("BackgroundColor", ofColor::fromHex(0x000000));
    gui->addBreak()->setHeight(20.0f);
    
    
    infoFolder = gui->addFolder("Infos", ofColor::white);
    labelImg = infoFolder->addLabel("Filename: " + ofToString(filename));
    labelImgSize = infoFolder->addLabel("ImageSize: " + ofToString(img.getWidth()) + " x " + ofToString(img.getHeight()));
    labelNum = infoFolder->addLabel("Number of Points: " + ofToString(numVerts));
    labelZ = infoFolder->addLabel("Zoom: ");
    infoFolder->addFRM();
    
    gui->addBreak()->setHeight(100.0f);
    buttonSave = gui->addButton("Save Image");
    gui->addFooter();
    
    buttonOpen->onButtonEvent(this, &ofApp::onButtonEvent);
    buttonSave->onButtonEvent(this, &ofApp::onButtonEvent);
    elevateDropDown->onDropdownEvent(this, &ofApp::onDropdownEvent);
    modeDropdown->onDropdownEvent(this, &ofApp::onDropdownEvent);
    colorPickBg->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    depthSlider->onSliderEvent(this, &ofApp::onSliderEvent);
    pointSlider->onSliderEvent(this, &ofApp::onSliderEvent);
    
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
//        pointSize = ofMap(easyCam.getDistance(), 0, 3500, maxPointSize, 1, true);
        string saveFile = filename + "_" + "Zoom_" + ofToString(easyCam.getDistance()) + "_" + "Point_" + ofToString(pointSliderVal) + ".jpg";
        largeOffscreenImage.begin();
            ofClear(bgColor);
            easyCam.begin();
                glPointSize(pointSliderVal*8);
                ofPushMatrix();
                ofTranslate(-img.getWidth()/2,-img.getHeight()/2);
                mesh.draw();
                ofPopMatrix();
            easyCam.end();
        largeOffscreenImage.end();
        
        ofPixels p;
        largeOffscreenImage.readToPixels(p);
        ofSaveImage(p, "exports/" + saveFile);
        cout << "saved file: " << saveFile <<"\n" << endl;
        
        ofRectangle bounds(0, 0, ofGetWidth(), ofGetHeight());
        ofRectangle target(0, 0, ofscreenW, ofscreenW);
        target.scaleTo(bounds);
        record = false;
    
    }else{
        easyCam.begin();
//        pointSize = ofMap(easyCam.getDistance(), maxZoom, minZoom, maxPointSize, 1, true);
        glPointSize(pointSliderVal);
            ofPushMatrix();
                ofTranslate(-img.getWidth()/2,-img.getHeight()/2);
                mesh.draw();
            ofPopMatrix();
        easyCam.end();
    }

    //    if(record){
    //        string filename = ofToString(counter);
    //        cout << "recording frame " << counter << "\n" << endl;
    //        ofSaveScreen(filename +".png");
    //        counter++;
    //    }
    //
    
    gui->draw();
        
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
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

//--------------------------------------------------------------
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

//--------------------------------------------------------------
void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult){
    ofLogVerbose("getName(): "  + openFileResult.getName());
    ofLogVerbose("getPath(): "  + openFileResult.getPath());
    
    ofFile file (openFileResult.getPath());
    
    if (file.exists()){
    
    ofLogVerbose("The file exists - now checking the type via file extension");
    string fileExtension = ofToUpper(file.getExtension());
    
    //We only want images
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




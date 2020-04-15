#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    filename = "t4";
    img.load(filename + ".jpg");
    
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
    
    buttonOpen = gui->addButton("Load Picture...");
    depthSlider = gui->addSlider("Depth", 0, 1000, depthSliderVal);
    colorPickBg = gui->addColorPicker("BackgroundColor", ofColor::fromHex(0x000000));
    gui->addFRM();
    
    buttonOpen->onButtonEvent(this, &ofApp::onButtonEvent);
    colorPickBg->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    depthSlider->onSliderEvent(this, &ofApp::onSliderEvent);
    
    bgColor = ofColor(0,0,0);

}


//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(bgColor);
    int maxPointSize = 10;
    int maxZoom = 200;
    int minZoom = 3500;
    
    if(record){
//        pointSize = ofMap(easyCam.getDistance(), 0, 3500, maxPointSize, 1, true);
        string saveFile = filename + "_" + "Zoom_" + ofToString(easyCam.getDistance()) + "_" + "Point_" + ofToString(pointSize) + "_" + ofToString(maxPointSize) + ".jpg";
        largeOffscreenImage.begin();
            ofClear(bgColor);
            easyCam.begin();
                glPointSize(pointSize*4);
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
        
        ofRectangle bounds(0,0, ofGetWidth(), ofGetHeight());
        ofRectangle target(0, 0, ofscreenW, ofscreenW);
        target.scaleTo(bounds);
    
    }else{
        easyCam.begin();
        pointSize = ofscreenH / img.getWidth();
//        pointSize = ofMap(easyCam.getDistance(), maxZoom, minZoom, maxPointSize, 1, true);
        glPointSize(1);
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
        if (key == 's'){
         record = true;
     }
    
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == 's'){
        record = false;
    }
}

//--------------------------------------------------------------
void ofApp::createMesh(){
    mesh.clear();
    int w = img.getWidth();
    int h = img.getHeight();

    for(int x = 0; x < w; x++){
        for(int y = 0; y < h; y++){
            ofColor clr = img.getColor(x, y);
            float intense = clr.getLightness();
            float saturation = clr.getSaturation();
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
    for (int i=0; i<numVerts; ++i) {
        ofVec3f vert = mesh.getVertex(i);
        ofColor clr = mesh.getColor(i);
        
        float saturation = clr.getSaturation();
        float z = ofMap(saturation, 0, 255, 0, depthSliderVal);

        vert.x = vert.x;
        vert.y = vert.y;
        vert.z = z;
        mesh.setVertex(i, vert);
    }
}

//--------------------------------------------------------------
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e){
    ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a picture");
    if (openFileResult.bSuccess){
        ofLogVerbose("User selected a file");
        processOpenFileSelection(openFileResult);
    }else {
        ofLogVerbose("User hit cancel");
    }
}

void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e){
    bgColor = e.color;
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e){
    depthSliderVal = depthSlider->getValue();
    updateMesh();


    
    
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
}




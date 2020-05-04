#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    clear.addListener(this, &ofApp::clearPressed);
    learn.addListener(this, &ofApp::learnPressed);
    save.addListener(this, &ofApp::savePressed);
    mode1.addListener(this, &ofApp::mode1Pressed);
    mode2.addListener(this, &ofApp::mode2Pressed);
    mode3.addListener(this, &ofApp::mode3Pressed);
    
    gui.setup();
    gui.add(threshold.setup("Threshold",100,0,300));
    gui.add(learn.setup("Learn Background"));
    gui.add(mode1.setup("Mode 1"));
    gui.add(mode2.setup("Mode 2"));
//    gui.add(mode3.setup)("Mode 3");
    gui.add(clear.setup("Clear"));
    gui.add(save.setup("Save Image"));
    
//    XML.setValue("settings:number", 11);
//    XML.saveFile("mySettings.xml");
    
    webcam.setup(ofGetWindowWidth(),ofGetWindowHeight());
//    webcam.setup(1024,768);

    
    //    webcam.setup(640, 480);
    //    // imitate() will set up bg and diff, so they have the same size and type as cam
    //    ofxCv::imitate(background, webcam);
    //    ofxCv::imitate(diff, webcam);
    
    drawMode = 1;
    color = ofColor(255,255,255,150);
    ofEnableAlphaBlending();
   

}

//--------------------------------------------------------------
void ofApp::update(){
    

    webcam.update();
    
    if(webcam.isFrameNew()){
        ofxCv::absdiff(webcam, background, diff);
        diff.update();
        finder.setSortBySize(true);
        finder.setThreshold(threshold);
        finder.findContours(diff);
        
    }
  
// How to fade out over time?
//        float time = ofGetElapsedTimef();
//        color = ofColor(255,255,255,255);
//        ofColor white = ofColor(255,255,255,255);
//        color.set(white);
//        ofColor fade = ofColor(255,255,255,0);
//        colorUpdate.lerp(fade,time);
//        ofSetColor(color);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // How to full screen?
    // How to grayscale video?
    // Why the saved screenshot is blue?
    ofSetColor(60);
    webcam.draw(0,0);
    ofSetColor(255);
    gui.draw();
    
    
    //    background.draw(640,0);
    //    diff.draw(0,480);
    
    ofSetLineWidth(3);
    
    
    switch (drawMode){
            
        // Outline Copy
        case 1: {
            
            if (finder.size() > 0){
 
                bodyLine = finder.getPolyline(0);
                bodyLine = bodyLine.getSmoothed(50);
                ofSetColor(color);
                bodyLine.draw();
                history.push_back(bodyLine);
                
                if(history.size()>30){
                    history.erase(history.begin());
                }
                
                for(int i=0; i<history.size();i+=3){
                    history[i].draw();
                }
                
                break;
            }
            
            
        // Outline Points Trail
        case 2: {
            if (finder.size() > 0){
                
                bodyLine = finder.getPolyline(0);
                bodyLine = bodyLine.getSmoothed(50);
//                bodyLine.draw();
                
                int numberOfPoints = 50;

                for(int i=0; i<numberOfPoints; i++){
                    trails.push_back(pointTrail);
                }

                int counter=0;
                ofSetColor(color);
                
                for (int p=0; p<100; p+= (100/numberOfPoints)) {
                    
                    ofVec3f point = bodyLine.getPointAtPercent(p/100.0);
                    float floatIndex = bodyLine.getIndexAtPercent(p/100.0);
//                    ofDrawCircle(point, 5);
                    
                    trails[counter].addVertex(point);
                    trails[counter] = trails[counter].getSmoothed(3);
                    trails[counter].draw();
                    
                    if(trails[counter].size()>70){
                        trails[counter].getVertices().erase( trails[counter].getVertices().begin());
                    }
                    counter++;

                }
                   
              
                    
//                    //Fill in color
//                    ofPath bodyLinePath;
//                    for(int i=0; i<bodyLine.getVertices().size();i++){
//                        if(i == 0){
//                            bodyLinePath.newSubPath();
//                            bodyLinePath.moveTo(bodyLine.getVertices()[i]);
//                        } else {
//                            bodyLinePath.lineTo(bodyLine.getVertices()[i]);
//                        }
//                    }
//                    bodyLinePath.close();
//                    //                bodyLinePath.setFillColor(255);
//                    bodyLinePath.draw();
//                }
                
  
                
            }
            break;
        }
            
        // PoseNet (TBD)
        case 3: {
                
            break;
        }
            
        }
            
            
    }
    
    
    
    //    ofPushMatrix();
    //    ofTranslate(640, 480);
    //    finder.draw();
    //    ofPopMatrix();
    

}

//--------------------------------------------------------------
void ofApp::clearPressed(){
    history.clear();
}
//--------------------------------------------------------------
void ofApp::learnPressed(){
    ofxCv::copy(webcam, background);
    background.update();
}
//--------------------------------------------------------------
void ofApp::savePressed(){
    screenShot.grabScreen(0,0,ofGetWindowWidth(),ofGetWindowHeight());
    screenShot.save(ofGetTimestampString() + ".jpg", OF_IMAGE_QUALITY_HIGH);
}
//--------------------------------------------------------------
void ofApp::mode1Pressed(){
    drawMode = 1;
    history.clear();
    trails.clear();
};
//--------------------------------------------------------------
void ofApp::mode2Pressed(){
    drawMode = 2;
    history.clear();
    trails.clear();
};
//--------------------------------------------------------------
void ofApp::mode3Pressed(){
    drawMode = 3;
    history.clear();
    trails.clear();
};
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == ' '){
        ofxCv::copy(webcam, background);
        background.update();
        history.clear();
        trails.clear();
    }
    
    if (key == 's' || key == 'S'){
        screenShot.grabScreen(0,0,ofGetWindowWidth(),ofGetWindowHeight());
        screenShot.save(ofGetTimestampString() + ".jpg", OF_IMAGE_QUALITY_HIGH);
    }
    
    if (key == 'l'){
        history.clear();
        trails.clear();
    }
    
    if (key == '1'){
        drawMode = 1;
        history.clear();
        trails.clear();
    }
    
    if (key == '2'){
        drawMode = 2;
        history.clear();
        trails.clear();
    }
    
    if (key == '3'){
        drawMode = 3;
        history.clear();
        trails.clear();
    }
    

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

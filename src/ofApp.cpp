#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    clear.addListener(this, &ofApp::clearPressed);
    learn.addListener(this, &ofApp::learnPressed);
    save.addListener(this, &ofApp::savePressed);
    mode1.addListener(this, &ofApp::mode1Pressed);
    mode2.addListener(this, &ofApp::mode2Pressed);
    mode3.addListener(this, &ofApp::mode3Pressed);
//    mode1.addListener(this, &ofApp::modePressed('1'));
//    mode2.addListener(this, &ofApp::modePressed('2'));
//    mode3.addListener(this, &ofApp::modePressed('3'));
    
    gui.setup();
    gui.add(threshold.setup("Threshold",100,0,300));
    gui.add(learn.setup("Learn Background"));
    gui.add(mode1.setup("Mode 1"));
    gui.add(mode2.setup("Mode 2"));
//    gui.add(mode3.setup)("Mode 3");
    gui.add(clear.setup("Clear"));
    gui.add(save.setup("Save Image"));
    
    ofBackground(0);

#ifdef USE_LIVE
//    screenRect = ofRectangle(0, 0, ofGetWidth(), ofGetHeight());
//    videoRect = ofRectangle(0, 0, webcam.getWidth(), webcam.getHeight());
//    videoFullscreenRect = videoRect;
//    videoFullscreenRect.scaleTo(screenRect, OF_ASPECT_RATIO_KEEP_BY_EXPANDING);
    webcam.setup(ofGetWindowWidth(),ofGetWindowHeight());
    
    
    //    webcam.setup(1024,768);
    //    webcam.setup(640, 480);
    //    // imitate() will set up bg and diff, so they have the same size and type as cam
    //    ofxCv::imitate(background, webcam);
    //    ofxCv::imitate(diff, webcam);
#else
    player.load("testDemo.mov");
    
    screenRect = ofRectangle(0, 0, ofGetWidth(), ofGetHeight());
    videoRect = ofRectangle(0, 0, player.getWidth(), player.getHeight());
    videoFullscreenRect = videoRect;
    videoFullscreenRect.scaleTo(screenRect, OF_ASPECT_RATIO_KEEP_BY_EXPANDING);
    
    player.play();
#endif
    
    drawMode = 1;
    color = ofColor(255,255,255,150);
    

}

//--------------------------------------------------------------
void ofApp::update(){
    
#ifdef USE_LIVE

    webcam.update();
    
    if(webcam.isFrameNew()){
        ofxCv::absdiff(webcam, background, diff);
        diff.update();
        finder.setSortBySize(true);
        finder.setThreshold(threshold);
        finder.findContours(diff);
        
    }

#else
    player.update();
    
    if(player.isFrameNew()){
        ofxCv::absdiff(player, background, diff);
        diff.update();
        finder.setSortBySize(true);
        finder.setThreshold(threshold);
        finder.findContours(diff);
    }
#endif
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // How to grayscale video?
    ofSetColor(60);
    
#ifdef USE_LIVE
    webcam.draw(0,0);
//    webcam.draw(videoFullscreenRect);

#else
//    player.draw(0,0);
    player.draw(videoFullscreenRect);

#endif
    
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
                
                //store the outline to an array of 30 lines maximum
                history.push_back(bodyLine);
                if(history.size()>30){
                    history.erase(history.begin());
                }
                
                //take the outline out of the array and add alpha effect
                for(int i=0; i<history.size(); i+=3){
                    float color = ofMap(i,0,history.size(),0,255);
                    ofSetColor(255,255,255,color);
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

                //create an array of trails
                for(int i=0; i<numberOfPoints; i++){
                    trails.push_back(pointTrail);
                }

                int counter=0;
                ofSetColor(color);
                //get evenly spaced points on the outline
                for (int p=0; p<100; p+= (100/numberOfPoints)) {
                    
                    ofVec3f point = bodyLine.getPointAtPercent(p/100.0);
                    float floatIndex = bodyLine.getIndexAtPercent(p/100.0);
//                    ofDrawCircle(point, 5);
                    //draw trail from each point no longer than 70
                    trails[counter].addVertex(point);
                    trails[counter] = trails[counter].getSmoothed(3);
//                    trails[counter].draw();
                    ofMesh myMesh;
                    myMesh.setMode(OF_PRIMITIVE_LINE_STRIP);
                    for(int j=0; j<trails[counter].size(); j++){
                        myMesh.addVertex(trails[counter][j]);
                        myMesh.addColor(ofColor(255,255,255,ofMap(j,0,trails[counter].size(),0,255)));
                        
                    }
                    myMesh.draw();
                    
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
    
#ifdef USE_LIVE
    ofxCv::copy(webcam, background);
#else
    ofxCv::copy(player, background);
#endif
    
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

//void ofApp::modePressed(int newDrawMode){
//    drawMode = newDrawMode;
//    history.clear();
//    trails.clear();
//}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == ' '){
        
#ifdef USE_LIVE
        ofxCv::copy(webcam, background);
#else
        ofxCv::copy(player, background);
#endif
        background.update();
        history.clear();
        trails.clear();
    }
    
    if (key == 's' || key == 'S'){
        screenShot.grabScreen(0,0,ofGetWindowWidth(),ofGetWindowHeight());
        screenShot.save(ofGetTimestampString() + ".png", OF_IMAGE_QUALITY_HIGH);
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
    
    
//    if (key == '1'|| key == '2' || key == '3'){
//        modePressed(key);
//    }

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

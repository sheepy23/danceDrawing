#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    clear.addListener(this, &ofApp::clearPressed);
    learn.addListener(this, &ofApp::learnPressed);
    save.addListener(this, &ofApp::savePressed);
    
    gui.setup();
    gui.add(learn.setup("Learn"));
    gui.add(threshold.setup("Threshold",100,0,300));
    gui.add(clear.setup("Clear"));
    gui.add(save.setup("Save Image"));
    
//    XML.setValue("settings:number", 11);
//    XML.saveFile("mySettings.xml");
    
    // How to full screen?
    webcam.setup(ofGetWindowWidth(),ofGetWindowHeight());
//    webcam.setup(1024,768);

    
    //    webcam.setup(640, 480);
    //    // imitate() will set up bg and diff, so they have the same size and type as cam
    //    ofxCv::imitate(background, webcam);
    //    ofxCv::imitate(diff, webcam);
    
    drawMode = 1;
    

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
            
            // Outline Trail
        case 1: {
            
            if (finder.size() > 0){
                
                
                bodyLine = finder.getPolyline(0);
                bodyLine = bodyLine.getSmoothed(50);
                bodyLine.draw();
                history.push_back(bodyLine);
                
                if(history.size()>30){
                    history.erase(history.begin());
                }
                
                for(int i=0; i<history.size();i++){
                    history[i].draw();
                    i = i+2;
                }
                break;
            }
            
            
            // Outline Points Trail (TBD)
            // How to get points on the outline of equal interval while the outline is updating?
            // How to draw the trail of each points?
        case 2: {
            if (finder.size() > 0){
                
                bodyLine = finder.getPolyline(0);
                bodyLine = bodyLine.getSmoothed(50);
                bodyLine.draw();
                
                for(int i=0; i<bodyLine.size(); i++){
                    points.push_back(bodyLine[i]);
                    ofDrawCircle(bodyLine[i],5);
                    ofSetColor(255);
                    i = i+100;
                }
                
                for(int i=0; i<points.size(); i++){
                    myLine.addVertex(points[i]);
                    myLine.draw();
                }
                
                
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
void ofApp::keyPressed(int key){
    
    if (key == ' '){
        ofxCv::copy(webcam, background);
        background.update();
    }
    
    if (key == 's' || key == 'S'){
        screenShot.grabScreen(0,0,ofGetWindowWidth(),ofGetWindowHeight());
        screenShot.save(ofGetTimestampString() + ".jpg", OF_IMAGE_QUALITY_HIGH);
    }
    
    if (key == 'l'){
        history.clear();
    }
    
    if (key == '1'){
        drawMode = 1;
        history.clear();
    }
    
    if (key == '2'){
        drawMode = 2;
        history.clear();
    }
    
    if (key == '3'){
        drawMode = 3;
        history.clear();
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

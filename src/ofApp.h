#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"

//#define USE_LIVE

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void learnPressed();
        void clearPressed();
        void savePressed();
//        void ofApp::modePressed();
        void mode1Pressed();
        void mode2Pressed();
        void mode3Pressed();
    

    ofxPanel gui;
    ofxIntSlider threshold;
    ofxButton clear;
    ofxButton learn;
    ofxButton save;
    ofxButton mode1;
    ofxButton mode2;
    ofxButton mode3;
    
    ofxXmlSettings XML;
    
    ofRectangle screenRect;
    ofRectangle videoRect;
    ofRectangle videoFullscreenRect;
    
 
#ifdef USE_LIVE
    ofVideoGrabber webcam;
#else
    ofVideoPlayer player;
#endif
    
    ofImage background;
    ofImage diff;
    ofRectangle original;
    ofxCv::ContourFinder finder;
    ofxCvGrayscaleImage processedImg;
    
    ofImage screenShot;
    

    ofPolyline bodyLine;
    vector <ofPoint> points; //array of points on an outline
    vector <ofPolyline> history; //array of outlines
//    ofPolyline myLine;
    ofPolyline pointTrail;
    vector <ofPolyline> trails;
    int drawMode;
    ofColor color;
    
};

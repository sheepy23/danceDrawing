#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"

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
    

    ofxPanel gui;
    ofxIntSlider threshold;
    ofxButton clear;
    ofxButton learn;
    ofxButton save;
    
    ofxXmlSettings XML;
    
    ofVideoGrabber webcam;
    ofImage background;
    ofImage diff;
    ofxCv::ContourFinder finder;
    ofxCvGrayscaleImage processedImg;
    
    ofImage screenShot;
    
    ofPolyline myLine;
    ofPolyline bodyLine;
    vector <ofPoint> points; //array of points on an outline
    vector <ofPolyline> history; //array of outlines
    int drawMode;
    
};

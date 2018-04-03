#pragma once

#include "ofMain.h"
#include "ofxBlui.h"

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

    ofxBlui blui;
    int root;
    int col;
    int colr;
    int option1;
    float progress1;
    float progress2;
    int enum1;

    void buttonPressed(int item, UIevent event);
    static void playPressed(int item, UIevent event);
};

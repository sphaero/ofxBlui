#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //blui = ofxBlui();
    //blui.test();

    //build ui content
    uiBeginLayout();

    //construct the root panel
    root = blui.panel();
    uiSetSize(0, 250, ofGetHeight());
    //((UIData*)uiGetHandle(root))->handler = roothandler;
    uiSetEvents(root, UI_SCROLL|UI_BUTTON0_DOWN);
    uiSetLayout(root, UI_HFILL|UI_TOP);
    uiSetBox(root, UI_COLUMN);

    int content = uiItem();
    uiSetLayout(content, UI_TOP);
    uiSetSize(content, 250, ofGetHeight());
    uiInsert(root, content);
    //construct content column for the root panel
    col = blui.column();
    uiInsert(content, col);
    uiSetMargins(col, 10,10,10,10);
    uiSetLayout(col, UI_TOP|UI_HFILL);

    //add buttons
    blui.column_append(col, blui.button(BND_ICON_GHOST, "Ghost 1", buttonPressed));
    blui.column_append(col, blui.button(BND_ICON_HELP, "Ghost 2", buttonPressed));
    blui.column_append(col, blui.button(BND_ICON_COLOR, "Ghost 3", buttonPressed));

    // add button group
    progress1 = 0.25f;
    progress2 = 0.75f;
    option1 = 1;
    static int option2 = 0;
    static int option3 = 0;
    {
        int rows = blui.column_append(col, blui.row());
        int coll = blui.row_append(rows, blui.vgroup());
        blui.vgroup_append(coll, blui.label(-1, "Items 4.0:"));
        coll = blui.vgroup_append(coll, blui.vbox());
        blui.vgroup_append(coll, blui.button(BND_ICON_GHOST, "Item 4.0.0", NULL));
        uiSetMargins(blui.vgroup_append(coll, blui.button(BND_ICON_GHOST, "Item 4.0.1", NULL)),0,-2,0,0);
        colr = blui.row_append(rows, blui.vgroup());
        uiSetMargins(colr, 8, 0, 0, 0);
        uiSetFrozen(colr, option1);
        blui.vgroup_append(colr, blui.label(-1, "Items 4.1:"));
        int colr2 = blui.vgroup_append(colr, blui.vbox());
        blui.vgroup_append(colr2, blui.slider("Item 4.1.0", &progress1));
        uiSetMargins(blui.vgroup_append(colr2, blui.slider("Item 4.1.1", &progress2)),0,-2,0,0);
    }

    blui.column_append(col, blui.check("Frozen", &option1));

    {
        int h = blui.column_append(col, blui.hbox());
        blui.hgroup_append(h, blui.radio(BND_ICON_GHOST, "Item 3.0", &enum1));
        uiSetMargins(blui.hgroup_append_fixed(h, blui.radio(BND_ICON_REC, NULL, &enum1)), -1,0,0,0);
        uiSetMargins(blui.hgroup_append_fixed(h, blui.radio(BND_ICON_PLAY, NULL, &enum1)), -1,0,0,0);
        uiSetMargins(blui.hgroup_append(h, blui.radio(BND_ICON_GHOST, "Item 3.3", &enum1)), -1,0,0,0);
    }

    static char txt[100] = "blabla";
    blui.column_append(col, blui.textbox(txt, 100));

    uiEndLayout();

}

//--------------------------------------------------------------
void ofApp::update(){
    //ofLogNotice() << option1 << ":" << colr;
    uiSetFrozen(colr, option1);
    blui.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    blui.draw();
}

void ofApp::buttonPressed(int item, UIevent event)
{
    ofLogNotice() << item << ":" << event;
    ofLogError() << this->col;
}

void ofApp::playPressed(int item, UIevent event)
{
    //UIRadioData *data = (UIRadioData *)uiGetHandle(item);
    //*data->value = item;
    BNDwidgetState state = (BNDwidgetState)uiGetState(item);
    if (state == BND_ACTIVE) {

        ofLogNotice() << "playing";
        return;
    }
    ofLogNotice() << "stopped";
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
    uiSetSize(0, 250, h);

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

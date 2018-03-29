#ifndef OFXBLUI_H
#define OFXBLUI_H
#pragma once
#include "ofMain.h"
#include "nanovg.h"
#include "blendish.h"
#include "oui.h"

class ofxBlui
{
public:
    typedef enum
    {
        // label
        ST_LABEL = 0,
        // button
        ST_BUTTON = 1,
        // radio button
        ST_RADIO = 2,
        // progress slider
        ST_SLIDER = 3,
        // column
        ST_COLUMN = 4,
        // row
        ST_ROW = 5,
        // check button
        ST_CHECK = 6,
        // panel
        ST_PANEL = 7,
        // text
        ST_TEXT = 8,
        //
        ST_IGNORE = 9,

        ST_DEMOSTUFF = 10,
        // colored rectangle
        ST_RECT = 11,

        ST_HBOX = 12,
        ST_VBOX = 13,
    } SubType;

    typedef struct
    {
        int subtype;
        UIhandler handler;
    } UIData;

    typedef struct
    {
        UIData head;
        const char *label;
        NVGcolor color;
    } UIRectData;

    typedef struct
    {
        UIData head;
        int iconid;
        const char *label;
    } UIButtonData;

    typedef struct
    {
        UIData head;
        const char *label;
        int *option;
    } UICheckData;

    typedef struct
    {
        UIData head;
        int iconid;
        const char *label;
        int *value;
    } UIRadioData;

    typedef struct
    {
        UIData head;
        const char *label;
        float *progress;
    } UISliderData;

    typedef struct
    {
        UIData head;
        char *text;
        int maxsize;
    } UITextData;

    ofxBlui();

    UIcontext *uictx;
    struct NVGcontext* vg = NULL;

    void init(NVGcontext *vg);
    int label(int iconid, const char *label);
    int button(int iconid, const char *label, UIhandler handler);
    int check(const char *label, int *option);
    int slider(const char *label, float *progress);
    int textbox(char *text, int maxsize);
    int radio(int iconid, const char *label, int *value);
    int radio(int iconid, const char *label, int *value, UIhandler handler);
    int panel();
    int hbox();
    int vbox();
    int column();
    int column_append(int parent, int item);
    int vgroup();
    int vgroup_append(int parent, int item);
    int hgroup();
    int hgroup_append(int parent, int item);
    int hgroup_append_fixed(int parent, int item);
    int row_append(int parent, int item);
    int row();

    void enableMouseEvents();
    void disableMouseEvents();
    void enableKeyEvents();
    void disableKeyEvents();

    void update();
    void draw();
    void test();
    int root;
    int col;

protected:
    void drawUIItems(NVGcontext *vg, int item, int corners);
    void drawUIItemsHbox(NVGcontext *vg, int item);
    void drawUIItemsVbox(NVGcontext *vg, int item);
    void drawUI(NVGcontext *vg, int item, int corners);

    //handlers
    static void checkhandler(int item, UIevent event);
    static void textboxhandler(int item, UIevent event);
    static void radiohandler(int item, UIevent event);
    static void sliderhandler(int item, UIevent event);
    static void ui_handler(int item, UIevent event);

    //events
    void _mouseMoved(ofMouseEventArgs &e);
    void _mousePressed(ofMouseEventArgs &e);
    void _mouseReleased(ofMouseEventArgs &e);
    void _mouseDragged(ofMouseEventArgs &e);

    void _keyPressed(ofKeyEventArgs &e);
    void _keyReleased(ofKeyEventArgs &e);

    //helpers
    void testrect(NVGcontext *vg, UIrect rect) {
    #if 0
        nvgBeginPath(vg);
        nvgRect(vg,rect.x+0.5,rect.y+0.5,rect.w-1,rect.h-1);
        nvgStrokeColor(vg,nvgRGBf(1,0,0));
        nvgStrokeWidth(vg,1);
        nvgStroke(vg);
    #endif
    }
};

#endif // OFXBLUI_H

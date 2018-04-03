#ifndef OFXBLUI_H
#define OFXBLUI_H
#pragma once
#include "ofMain.h"
#include "nanovg.h"
#include "blendish.h"
#include "oui.h"

/*** helpers to use CPP members as C callbacks
 *
 * This is really some advanced cpp shit. I can hardly grasp what it is doing
 * but in the end it just converts a member method to a c callback pointer
 *
 * This comes from many examples on SO and code from ofEvents (openFrameworks)
 */
template <typename T>
struct Callback;

template <typename Ret, typename... Params>
struct Callback<Ret(Params...)> {
    template <typename... Args>
    static Ret callback(Args... args) { return func(args...); }
    static std::function<Ret(Params...)> func;
};

// Initialize the static member.
template <typename Ret, typename... Params>
std::function<Ret(Params...)> Callback<Ret(Params...)>::func;

template <class InstanceClass>
UIhandler ofxBluiCptr(InstanceClass  * instance, void (InstanceClass::*instanceMethod)(int, UIevent))
{
    Callback<void(int, UIevent)>::func = std::bind( instanceMethod, instance, std::placeholders::_1, std::placeholders::_2 );
    void (*c_func)(int, UIevent) = static_cast<decltype(c_func)>(Callback<void(int, UIevent)>::callback);
    return c_func;
}
// end helpers

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
    // because of the template this needs to be implemented in the header https://stackoverflow.com/questions/10632251/undefined-reference-to-template-function
    template <class InstanceClass>
    int button(int iconid, const char *label, InstanceClass* instance, void (InstanceClass::*instanceMethod)(int, UIevent))
    {
        UIhandler handler = ofxBluiCptr(instance, instanceMethod);
        return button(iconid, label, handler);
    }

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

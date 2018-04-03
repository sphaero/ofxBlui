#include "ofxBlui.h"
#define NANOVG_GL2_IMPLEMENTATION
#include "nanovg_gl.h"
//#ifndef BLENDISH_IMPLEMENTATION
#define BLENDISH_IMPLEMENTATION
#include "blendish.h"
//#endif
//#ifndef OUI_IMPLEMENTATION
#define OUI_IMPLEMENTATION
#include "oui.h"
//#endif


ofxBlui::ofxBlui()
{
    vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
    if (vg == NULL) {
        printf("Could not init nanovg. Probably no OpenGL2 support?\n");
        ofExit(-1);
    }
    init(vg);
    uictx = uiCreateContext(4096, 1<<20);
    uiMakeCurrent(uictx);
    uiSetHandler(ui_handler);

    //setup listeners for events
    enableMouseEvents();
    enableKeyEvents();
}

void ofxBlui::update()
{
    uiProcess(int(ofGetElapsedTimeMillis()));
}

//--------------------------------------------------------------
void ofxBlui::draw()
{
    nvgBeginFrame(vg, ofGetWidth(), ofGetHeight(), 1.0);
    //bndBackground(vg, 0, 0, ofGetWidth(), ofGetHeight());
    drawUI(vg, 0, BND_CORNER_NONE);
    nvgEndFrame(vg);
}

void ofxBlui::enableMouseEvents()
{
    ofAddListener(ofEvents().mousePressed, this, &ofxBlui::_mousePressed);
    ofAddListener(ofEvents().mouseMoved, this, &ofxBlui::_mouseMoved);
    ofAddListener(ofEvents().mouseDragged, this, &ofxBlui::_mouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &ofxBlui::_mouseReleased);
}

void ofxBlui::enableKeyEvents() {

    ofAddListener(ofEvents().keyPressed, this, &ofxBlui::_keyPressed);
    ofAddListener(ofEvents().keyReleased, this, &ofxBlui::_keyReleased);
}

void ofxBlui::init(NVGcontext *vg)
{
    bndSetFont(nvgCreateFont(vg, "system", "data/DejaVuSans.ttf"));
    bndSetIconImage(nvgCreateImage(vg, "data/blender_icons16.png", 0));
}

int ofxBlui::label(int iconid, const char *label)
{
    int item = uiItem();
    uiSetSize(item, 0, BND_WIDGET_HEIGHT);
    UIButtonData *data = (UIButtonData *)uiAllocHandle(item, sizeof(UIButtonData));
    data->head.subtype = ST_LABEL;
    data->head.handler = NULL;
    data->iconid = iconid;
    data->label = label;
    return item;
}

int ofxBlui::button(int iconid, const char *label, UIhandler handler)
{
    // create new ui item
    int item = uiItem();
    // set size of wiget; horizontal size is dynamic, vertical is fixed
    uiSetSize(item, 0, BND_WIDGET_HEIGHT);
    uiSetEvents(item, UI_BUTTON0_HOT_UP);
    // store some custom data with the button that we use for styling
    UIButtonData *data = (UIButtonData *)uiAllocHandle(item, sizeof(UIButtonData));
    data->head.subtype = ST_BUTTON;
    data->head.handler = handler;
    data->iconid = iconid;
    data->label = label;
    return item;
}

int ofxBlui::check(const char *label, int *option)
{
    // create new ui item
    int item = uiItem();
    // set size of wiget; horizontal size is dynamic, vertical is fixed
    uiSetSize(item, 0, BND_WIDGET_HEIGHT);
    // attach event handler e.g. demohandler above
    uiSetEvents(item, UI_BUTTON0_DOWN);
    // store some custom data with the button that we use for styling
    UICheckData *data = (UICheckData *)uiAllocHandle(item, sizeof(UICheckData));
    data->head.subtype = ST_CHECK;
    data->head.handler = checkhandler;
    data->label = label;
    data->option = option;
    return item;
}

int ofxBlui::ofxBlui::slider(const char *label, float *progress)
{
    // create new ui item
    int item = uiItem();
    // set size of wiget; horizontal size is dynamic, vertical is fixed
    uiSetSize(item, 0, BND_WIDGET_HEIGHT);
    // attach our slider event handler and capture two classes of events
    uiSetEvents(item, UI_BUTTON0_DOWN | UI_BUTTON0_CAPTURE);
    // store some custom data with the button that we use for styling
    // and logic, e.g. the pointer to the data we want to alter.
    UISliderData *data = (UISliderData *)uiAllocHandle(item, sizeof(UISliderData));
    data->head.subtype = ST_SLIDER;
    data->head.handler = sliderhandler;
    data->label = label;
    data->progress = progress;
    return item;
}

int ofxBlui::ofxBlui::textbox(char *text, int maxsize)
{
    int item = uiItem();
    uiSetSize(item, 0, BND_WIDGET_HEIGHT);
    uiSetEvents(item, UI_BUTTON0_DOWN | UI_KEY_DOWN | UI_CHAR);
    // store some custom data with the button that we use for styling
    // and logic, e.g. the pointer to the data we want to alter.
    UITextData *data = (UITextData *)uiAllocHandle(item, sizeof(UITextData));
    data->head.subtype = ST_TEXT;
    data->head.handler = textboxhandler;
    data->text = text;
    data->maxsize = maxsize;
    return item;
}

int ofxBlui::radio(int iconid, const char *label, int *value)
{
    return radio(iconid, label, value, radiohandler);
/*    int item = uiItem();
    uiSetSize(item, label?0:BND_TOOL_WIDTH, BND_WIDGET_HEIGHT);
    UIRadioData *data = (UIRadioData *)uiAllocHandle(item, sizeof(UIRadioData));
    data->head.subtype = ST_RADIO;
    data->head.handler = radiohandler;
    data->iconid = iconid;
    data->label = label;
    data->value = value;
    uiSetEvents(item, UI_BUTTON0_DOWN);
    return item;*/
}

int ofxBlui::radio(int iconid, const char *label, int *value, UIhandler handler)
{
    int item = uiItem();
    uiSetSize(item, label?0:BND_TOOL_WIDTH, BND_WIDGET_HEIGHT);
    UIRadioData *data = (UIRadioData *)uiAllocHandle(item, sizeof(UIRadioData));
    data->head.subtype = ST_RADIO;
    data->head.handler = handler;
    data->iconid = iconid;
    data->label = label;
    data->value = value;
    uiSetEvents(item, UI_BUTTON0_DOWN);
    return item;
}

int ofxBlui::panel()
{
    int item = uiItem();
    UIData *data = (UIData *)uiAllocHandle(item, sizeof(UIData));
    data->subtype = ST_PANEL;
    data->handler = NULL;
    return item;
}

int ofxBlui::hbox()
{
    int item = uiItem();
    UIData *data = (UIData *)uiAllocHandle(item, sizeof(UIData));
    data->subtype = ST_HBOX;
    data->handler = NULL;
    uiSetBox(item, UI_ROW);
    return item;
}


int ofxBlui::vbox()
{
    int item = uiItem();
    UIData *data = (UIData *)uiAllocHandle(item, sizeof(UIData));
    data->subtype = ST_VBOX;
    data->handler = NULL;
    uiSetBox(item, UI_COLUMN);
    return item;
}


int ofxBlui::column_append(int parent, int item)
{
    uiInsert(parent, item);
    // fill parent horizontally, anchor to previous item vertically
    uiSetLayout(item, UI_HFILL);
    uiSetMargins(item, 0, 1, 0, 0);
    return item;
}

int ofxBlui::column()
{
    int item = uiItem();
    uiSetBox(item, UI_COLUMN);
    return item;
}

int ofxBlui::vgroup_append(int parent, int item)
{
    uiInsert(parent, item);
    // fill parent horizontally, anchor to previous item vertically
    uiSetLayout(item, UI_HFILL);
    return item;
}

int ofxBlui::vgroup()
{
    int item = uiItem();
    uiSetBox(item, UI_COLUMN);
    return item;
}

int ofxBlui::hgroup_append(int parent, int item)
{
    uiInsert(parent, item);
    uiSetLayout(item, UI_HFILL);
    return item;
}

int ofxBlui::hgroup_append_fixed(int parent, int item)
{
    uiInsert(parent, item);
    return item;
}

int ofxBlui::hgroup()
{
    int item = uiItem();
    uiSetBox(item, UI_ROW);
    return item;
}

int ofxBlui::row_append(int parent, int item)
{
    uiInsert(parent, item);
    uiSetLayout(item, UI_HFILL);
    return item;
}

int ofxBlui::row()
{
    int item = uiItem();
    uiSetBox(item, UI_ROW);
    return item;
}

// handlers
void ofxBlui::checkhandler(int item, UIevent event)
{
    const UICheckData *data = (const UICheckData *)uiGetHandle(item);
    *data->option = !(*data->option);
}

// starting offset of the currently active slider
static float sliderstart = 0.0;

// event handler for slider (same handler for all sliders)
void ofxBlui::sliderhandler(int item, UIevent event)
{
    // retrieve the custom data we saved with the slider
    UISliderData *data = (UISliderData *)uiGetHandle(item);

    switch(event) {
        default: break;
        case UI_BUTTON0_DOWN: {
            // button was pressed for the first time; capture initial
            // slider value.
            sliderstart = *data->progress;
        } break;
        case UI_BUTTON0_CAPTURE: {
            // called for every frame that the button is pressed.
            // get the delta between the click point and the current
            // mouse position
            UIvec2 pos = uiGetCursorStartDelta();
            // get the items layouted rectangle
            UIrect rc = uiGetRect(item);
            // calculate our new offset and clamp
            float value = sliderstart + ((float)pos.x / (float)rc.w);
            value = (value<0)?0:(value>1)?1:value;
            // assign the new value
            *data->progress = value;
        } break;
    }
}

void ofxBlui::textboxhandler(int item, UIevent event)
{
    UITextData *data = (UITextData *)uiGetHandle(item);
    switch(event) {
        default: break;
        case UI_BUTTON0_DOWN: {
            uiFocus(item);
        } break;
        case UI_KEY_DOWN: {
            unsigned int key = uiGetKey();
            ofLogNotice() << key;
            switch(key) {
                default: break;
                case OF_KEY_BACKSPACE: {
                    int size = strlen(data->text);
                    if (!size) return;
                    data->text[size-1] = 0;
                } break;
                case OF_KEY_RETURN: {
                    uiFocus(-1);
                } break;
            }
        } break;
        case UI_CHAR: {
            unsigned int key = uiGetKey();
            if ((key > 255)||(key < 32)) return;
            int size = strlen(data->text);
            if (size >= (data->maxsize-1)) return;
            data->text[size] = (char)key;
        } break;
    }
}

// simple logic for a radio button
void ofxBlui::radiohandler(int item, UIevent event)
{
    UIRadioData *data = (UIRadioData *)uiGetHandle(item);
    *data->value = item;
}

void ofxBlui::drawUIItems(NVGcontext *vg, int item, int corners)
{
    int kid = uiFirstChild(item);
    while (kid > 0) {
        drawUI(vg, kid, corners);
        kid = uiNextSibling(kid);
    }
}

void ofxBlui::drawUIItemsHbox(NVGcontext *vg, int item)
{
    int kid = uiFirstChild(item);
    if (kid < 0) return;
    int nextkid = uiNextSibling(kid);
    if (nextkid < 0)
    {
        drawUI(vg, kid, BND_CORNER_NONE);
    } else
    {
        drawUI(vg, kid, BND_CORNER_RIGHT);
        kid = nextkid;
        while (uiNextSibling(kid) > 0)
        {
            drawUI(vg, kid, BND_CORNER_ALL);
            kid = uiNextSibling(kid);
        }
        drawUI(vg, kid, BND_CORNER_LEFT);
    }
}

void ofxBlui::drawUIItemsVbox(NVGcontext *vg, int item)
{
    int kid = uiFirstChild(item);
    if (kid < 0) return;
    int nextkid = uiNextSibling(kid);
    if (nextkid < 0)
    {
        drawUI(vg, kid, BND_CORNER_NONE);
    } else
    {
        drawUI(vg, kid, BND_CORNER_DOWN);
        kid = nextkid;
        while (uiNextSibling(kid) > 0)
        {
            drawUI(vg, kid, BND_CORNER_ALL);
            kid = uiNextSibling(kid);
        }
        drawUI(vg, kid, BND_CORNER_TOP);
    }
}

void ofxBlui::drawUI(NVGcontext *vg, int item, int corners)
{
    const UIData *head = (const UIData *)uiGetHandle(item);
    UIrect rect = uiGetRect(item);
    if (uiGetState(item) == UI_FROZEN) {
        nvgGlobalAlpha(vg, BND_DISABLED_ALPHA);
    }
    if (head) {
        switch(head->subtype) {
            default: {
                testrect(vg,rect);
                drawUIItems(vg,item,corners);
            } break;
            case ST_HBOX: {
                drawUIItemsHbox(vg, item);
            } break;
            case ST_VBOX: {
                drawUIItemsVbox(vg, item);
            } break;
            case ST_PANEL: {
                bndBackground(vg, rect.x, rect.y, rect.w, rect.h);
                bndBevel(vg,rect.x,rect.y,rect.w,rect.h);
                drawUIItems(vg,item,corners);
            } break;
            case ST_LABEL: {
                assert(head);
                const UIButtonData *data = (UIButtonData*)head;
                bndLabel(vg,rect.x,rect.y,rect.w,rect.h,
                    data->iconid,data->label);
            } break;
            case ST_BUTTON: {
                const UIButtonData *data = (UIButtonData*)head;
                bndToolButton(vg,rect.x,rect.y,rect.w,rect.h,
                        corners,(BNDwidgetState)uiGetState(item),
                    data->iconid,data->label);
            } break;
            case ST_CHECK: {
                const UICheckData *data = (UICheckData*)head;
                BNDwidgetState state = (BNDwidgetState)uiGetState(item);
                if (*data->option)
                    state = BND_ACTIVE;
                bndOptionButton(vg,rect.x,rect.y,rect.w,rect.h, state,
                    data->label);
            } break;
            case ST_RADIO:{
                const UIRadioData *data = (UIRadioData*)head;
                BNDwidgetState state = (BNDwidgetState)uiGetState(item);
                if (*data->value == item)
                    state = BND_ACTIVE;
                bndRadioButton(vg,rect.x,rect.y,rect.w,rect.h,
                       corners,state,
                   data->iconid,data->label);
            } break;
            case ST_SLIDER:{
               const UISliderData *data = (UISliderData*)head;
               BNDwidgetState state = (BNDwidgetState)uiGetState(item);
               static char value[32];
               sprintf(value,"%.0f%%",(*data->progress)*100.0f);
               bndSlider(vg,rect.x,rect.y,rect.w,rect.h,
                       corners,state,
                   *data->progress,data->label,value);
            } break;
            case ST_TEXT: {
               const UITextData *data = (UITextData*)head;
               BNDwidgetState state = (BNDwidgetState)uiGetState(item);
               int idx = strlen(data->text);
               bndTextField(vg,rect.x,rect.y,rect.w,rect.h,
                       corners,state, -1, data->text, idx, idx);
            } break;
            case ST_DEMOSTUFF: {
               //draw_demostuff(vg, rect.x, rect.y, rect.w, rect.h);
            } break;
            case ST_RECT: {
                const UIRectData *data = (UIRectData*)head;
                if (rect.w && rect.h) {
                    BNDwidgetState state = (BNDwidgetState)uiGetState(item);
                    nvgSave(vg);
                    nvgStrokeColor(vg, nvgRGBAf(data->color.r,data->color.g,data->color.b,0.9f));
                    if (state != BND_DEFAULT) {
                        nvgFillColor(vg, nvgRGBAf(data->color.r,data->color.g,data->color.b,0.5f));
                    } else {
                        nvgFillColor(vg, nvgRGBAf(data->color.r,data->color.g,data->color.b,0.1f));
                    }
                    nvgStrokeWidth(vg,2);
                    nvgBeginPath(vg);
                    #if 0
                    nvgRect(vg,rect.x,rect.y,rect.w,rect.h);
                    #else
                    nvgRoundedRect(vg,rect.x,rect.y,rect.w,rect.h,3);
                    #endif
                    nvgFill(vg);
                    nvgStroke(vg);

                    if (state != BND_DEFAULT) {
                        nvgFillColor(vg, nvgRGBAf(0.0f,0.0f,0.0f,1.0f));
                        nvgFontSize(vg, 15.0f);
                        nvgBeginPath(vg);
                        nvgTextAlign(vg, NVG_ALIGN_TOP|NVG_ALIGN_CENTER);
                        nvgTextBox(vg, rect.x, rect.y+rect.h*0.3f, rect.w, data->label, NULL);
                    }

                    nvgRestore(vg);
                }
                nvgSave(vg);
                nvgIntersectScissor(vg, rect.x, rect.y, rect.w, rect.h);

                drawUIItems(vg,item,corners);

                nvgRestore(vg);
            } break;
        }
    } else {
        testrect(vg,rect);
        drawUIItems(vg,item,corners);
    }

    if (uiGetState(item) == UI_FROZEN) {
        nvgGlobalAlpha(vg, 1.0);
    }
}

void ofxBlui::ui_handler(int item, UIevent event)
{
    UIData *data = (UIData *)uiGetHandle(item);
    if (data && data->handler) {
        data->handler(item, event);
    }
}

void ofxBlui::test()
{
    //build ui content
    uiBeginLayout();

    //construct the root panel
    root = panel();
    uiSetSize(0,200,ofGetHeight());
    //((UIData*)uiGetHandle(root))->handler = roothandler;
    uiSetEvents(root, UI_SCROLL|UI_BUTTON0_DOWN);
    uiSetBox(root, UI_COLUMN);

    //construct content column for the root panel
    col = column();
    uiInsert(root, col);
    uiSetMargins(col, 10,10,10,10);
    uiSetLayout(col, UI_TOP|UI_HFILL);

    //add buttons
    column_append(col, button(BND_ICON_GHOST, "Ghost 1", NULL));
    column_append(col, button(BND_ICON_HELP, "Ghost 2", NULL));
    column_append(col, button(BND_ICON_COLOR, "Ghost 3", NULL));

    static char textbuffer[1024] = "The quick brown fox.";
    column_append(col, textbox(textbuffer, 1024));

    uiEndLayout();
}

void ofxBlui::_mouseMoved(ofMouseEventArgs &e)
{
    uiSetCursor(e.x, e.y);
}

void ofxBlui::_mousePressed(ofMouseEventArgs &e)
{
    uiSetButton(e.button, 0, 1);
}

void ofxBlui::_mouseReleased(ofMouseEventArgs &e)
{
    uiSetButton(e.button, 0, 0);
}

void ofxBlui::_mouseDragged(ofMouseEventArgs &e)
{
    uiSetCursor(e.x, e.y);
}

void ofxBlui::_keyPressed(ofKeyEventArgs &e) {

    uiSetKey(e.key, e.modifiers, 1);
    uiSetChar(e.key);
}

void ofxBlui::_keyReleased(ofKeyEventArgs &e) {

    uiSetKey(e.key, e.modifiers, 0);
}

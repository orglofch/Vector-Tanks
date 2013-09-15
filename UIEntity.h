/**
*
* @author: Owen Glofcheski
* @date: March 18, 2012
*
*/

#ifndef UIEntity_H_INCLUDED
#define UIEntity_H_INCLUDED

#include <vector>
#include <sstream>
#include <string>

#include <gl/gl.h>
#include <gl/glext.h>
#include "glfont.h"
#include <SDL/SDL.h>

#include "Vect.h"
#include "Color.h"
#include "Utility.h"
#include "Globals.h"

enum UIAction {
    HIDE = 0,
    SHOW = 1,
    SLIDEUPHIDE = 2,
    SLIDEUPSHOW = 3,
    SLIDEDOWNHIDE = 4,
    SLIDEDOWNSHOW = 5,
    SLIDELEFTHIDE = 6,
    SLIDELEFTSHOW = 7,
    SLIDERIGHTHIDE = 8,
    SLIDERIGHTSHOW = 9,
    FADEOUT = 10,
    FADEIN = 11,
    NONE = 12,
    TOGGLE = 13,
    TOGGLEUPSLIDE = 14,
    TOGGLEDOWNSLIDE = 15,
    TOGGLELEFTSLIDE = 16,
    TOGGLERIGHTSLIDE = 17,
    TOGGLEFADE = 18
};

class UIEntity
{
protected:
    // ToolTip
    UIEntity *toolTip;
    Vect hoverPos;
    int hoverTime;

    Color defaultColor;
    Color color;

    Vect pos; // Top Left Corner of texture box

    int endSteps, currentSteps;

    float endWidth, endHeight;
    float currentWidth, currentHeight;
    float widthInc, heightInc;

    float endAlpha;
    float currentAlpha;
    float alphaInc;

    UIAction action;
    bool isHidden;
    bool isClicked;
    bool isHovered;

    GLuint texture;
    GLuint defaultTexture;
    GLuint depressedTexture;

    void hideStep();
    void slideUpStep();
    void slideDownStep();
    void slideLeftStep();
    void slideRightStep();
    void fadeStep();

    void reset(float speed);
public:
    UIEntity(Vect pos, float width, float height, float alpha, bool isHidden, UIAction hideAction);
    ~UIEntity();

    void setPos(Vect pos);
    void setSize(Vect size);
    void setAlpha(float alpha);
    void setToolTip(UIEntity *toolTip);

    void setDefaultTexture(char *filename);
    void setDepressedTexture(char *filename);

    Vect getPos();
    Vect getSize();
    float getAlpha();

    bool checkCollide(Vect pos);

    // Hide
    void hide(float speed);
    void show(float speed);
    void toggle(float speed);

    // Slide Up
    void slideUpHide(float speed);
    void slideUpShow(float speed);
    void toggleUpSlide(float speed);

    // Slide Down
    void slideDownHide(float speed);
    void slideDownShow(float speed);
    void toggleDownSlide(float speed);

    // Slide Left
    void slideLeftHide(float speed);
    void slideLeftShow(float speed);
    void toggleLeftSlide(float speed);

    // Slide Right
    void slideRightHide(float speed);
    void slideRightShow(float speed);
    void toggleRightSlide(float speed);

    // Fade
    void fadeOut(float speed);
    void fadeIn(float speed);
    void toggleFade(float speed);

    void draw(Vect pos, float alpha);
    virtual void altDraw(Vect pos, float alpha) = 0;

    void step(Vect pos);
    virtual void altStep(Vect pos) = 0;
    void toolTipStep(Vect pos);

    void onClick(Vect pos);
    void onRelease(Vect pos);
    void onHover(Vect pos);
    void offHover(Vect pos);

    virtual void onClickAction(Vect pos) = 0;
    virtual void onReleaseAction(Vect pos) = 0;
    virtual void onHoverAction(Vect pos) = 0;
    virtual void offHoverAction(Vect pos) = 0;

};

class Button : public UIEntity
{
private:
    bool isDepressed;

    std::vector <UIEntity *> actionEntities; /** Possibly make on single triple structure **/
    std::vector <UIAction> actions;
    std::vector <float> speeds;
public:
    Button(Vect pos, float width, float height, float alpha, bool isHidden, UIAction hideAction);

    void addEntity(UIEntity *actionEntity, UIAction action, float speed);
    void addEntity(UIEntity *actionEntity, UIAction action);

    void altDraw(Vect pos, float alpha);
    void altStep(Vect pos);

    void onClickAction(Vect pos);
    void onReleaseAction(Vect pos);
    void onHoverAction(Vect pos);
    void offHoverAction(Vect pos);
};

class Container : public UIEntity
{
private:
    std::vector <UIEntity *> items;
    Vect padding;
    bool hasAbsolutePos;
public:
    Container(Vect pos, float width, float height, float alpha, bool isHidden, UIAction hideAction, bool hasaAbsolutePos);
    ~Container();

    void setPadding(const Vect &padding);
    void addEntity(UIEntity *item);

    void altDraw(Vect pos, float alpha);
    void altStep(Vect pos);

    void onClickAction(Vect pos);
    void onReleaseAction(Vect pos);
    void onHoverAction(Vect pos);
    void offHoverAction(Vect pos);
};

class ToolTip : public UIEntity
{
private:
public:
    ToolTip(float width, float height, float alpha, bool isHidden, UIAction hideAction);

    void altDraw(Vect pos, float alpha);
    void altStep(Vect pos);

    void onClickAction(Vect pos);
    void onReleaseAction(Vect pos);
    void onHoverAction(Vect pos);
    void offHoverAction(Vect pos);
};

class PercentageBar : public UIEntity
{
private:
    float *attribute;
    float attributeMax;
public:
    PercentageBar(Vect pos, float width, float height, float alpha, bool isHidden, float *attribute, float attributeMax, UIAction hideAction);

    void altDraw(Vect pos, float alpha);
    void altStep(Vect pos);

    void onClickAction(Vect pos);
    void onReleaseAction(Vect pos);
    void onHoverAction(Vect pos);
    void offHoverAction(Vect pos);
};

class ScrollBar : public UIEntity
{
private:
public:
    ScrollBar(UIEntity *partOf, UIAction hideAction);

    void altDraw(Vect pos, float alpha);
    void altStep(Vect pos);

    void onClickAction(Vect pos);
    void onReleaseAction(Vect pos);
    void onHoverAction(Vect pos);
    void offHoverAction(Vect pos);
};

class Timer : public UIEntity
{
private:
    bool isPaused;

    GLuint timerTexture;
    PixelPerfectGLFont timerFont;
    Uint32 startTime;
    Uint32 endTime;
    Uint32 turnLength;

    void startTimer();
    void pauseTimer();
    void togglePause();
public:
    Timer(Vect pos, float width, float height, float alpha, bool isHidden, UIAction hideAction);

    void altDraw(Vect pos, float alpha);
    void altStep(Vect pos);

    void onClickAction(Vect pos);
    void onReleaseAction(Vect pos);
    void onHoverAction(Vect pos);
    void offHoverAction(Vect pos);

    void resetTimer();
    bool checkTimer();
};

class TextArea: public UIEntity
{
private:
    GLuint textTexture;
    PixelPerfectGLFont textFont;

    std::string text;
public:
    TextArea(Vect pos, float width, float height, float alpha, bool isHidden, UIAction hideAction);

    void setText(std::string text);

    void altDraw(Vect pos, float alpha);
    void altStep(Vect pos);

    void onClickAction(Vect pos);
    void onReleaseAction(Vect pos);
    void onHoverAction(Vect pos);
    void offHoverAction(Vect pos);
};

#endif // UIEntity_H_INCLUDED

/**
*
* @author: Owen Glofcheski
* @date: February 2, 2012
*
*/

#ifndef PIXEDIT_H_INCLUDED
#define PIXEDIT_H_INCLUDED

#include <utility>
#include <gl/gl.h>
#include <cstdlib>

#include "Globals.h"
#include "Effects.h"
#include "Vect.h"
#include "Math.h"

class PixMap;

class PixEdit
{
private:
protected:
    Vect pos;

    PixMap *pixMap;

    std::pair <int, int> getXBound(float pos, float radius);
    std::pair <int, int> getYBound(float pos, float radius);
public:
    PixEdit(PixMap *pixMap, Vect pos);

    virtual void draw() = 0; /** Remove draw method if unused **/
    virtual bool step() = 0;

    virtual void onFinish() = 0;
};

class RemoveCircle : public PixEdit
{
private:
    int shakeCounter;
    int shakeAngle;
    int endRadius;
    int currentRadius;

    Color color;
public:
    RemoveCircle(PixMap *pixMap, Vect pos, int radius);

    void draw();
    bool step();

    void onFinish();
};

class CollapseCircle : public PixEdit
{
private:
    int endRadius;
    int * colBottom; // Bottoms of collapsing columns

    void findBottoms();
public:
    CollapseCircle(PixMap *pixMap, Vect pos, int radius);

    void draw();
    bool step();

    void onFinish();
};

class BuildSpike : public PixEdit
{
private:
    float heightInc;
    int radius;
    int currentWidth;
    float currentHeight;

    int buildCounter;
public:
    BuildSpike(PixMap *pixMap, Vect pos, int height, int radius);

    void draw();
    bool step();

    void onFinish();
};

class LiquifyCircle : public PixEdit
{
private:
    int endRadius;
    int currentRadius;
public:
    LiquifyCircle(PixMap *pixMap, Vect pos, int radius);

    void draw();
    bool step();

    void onFinish();
};

class BuildWall : public PixEdit
{
private:
    int endHeight;
    int currentHeight;
    int width;
public:
    BuildWall(PixMap *pixMap, Vect pos, int width, int height);

    void draw();
    bool step();

    void onFinish();
};

#endif // PIXEDIT_H_INCLUDED

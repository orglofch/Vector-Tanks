/**
*
* @author: Owen Glofcheski
* @date: January 31, 2012
*
*/

#ifndef PIXNODE_H_INCLUDED
#define PIXNODE_H_INCLUDED

#include "Globals.h"
#include "PixMap.h"
#include "Effects.h"
#include "Color.h"
#include "Vect.h"

class PixMap;

enum moveDir {
    DOWN = 0,
    LEFT = 1,
    RIGHT = 2
};

enum nodeType {
    DIRT = 0,
    SAND = 1,
    WATER = 2,
    LAVA = 3,
    SNOW = 4
};

class PixNode /** Make inhereted pixNode types **/
{
protected:
    PixMap *pixMap;

    float density; // do something with this

    bool isActive;
    bool isPartOf;
    bool isLiquid;
    //Object partOf;

    Vect pos;
    Color color;

    nodeType type;

    virtual void onDown() = 0;
    virtual void onLeft() = 0;
    virtual void onRight() = 0;
public:
    PixNode();

    void setPos(Vect pos);
    void setCol(Color color);
    void setPixMap(PixMap *pixMap);

    Color getCol();
    Vect getPos();
    int getType();

    void makeActive();
    void makeInactive();

    virtual bool canActive(Vect pos) = 0;
    bool checkActive();
    bool checkLiquid();

    void draw(); /** Possibly make virtual **/
    void step();
};

class Liquid : public PixNode
{
protected:
    Vect findActiveSwap();

    virtual void onDown() = 0;
    virtual void onLeft() = 0;
    virtual void onRight() = 0;
public:
    Liquid();

    virtual bool canActive(Vect pos) = 0;
};

class Solid : public PixNode
{
protected:
    virtual void onDown() = 0;
    virtual void onLeft() = 0;
    virtual void onRight() = 0;
public:
    Solid();

    virtual bool canActive(Vect pos) = 0;
};

class Dirt : public Solid
{
private:
    void onDown();
    void onLeft();
    void onRight();
public:
    Dirt();

    bool canActive(Vect pos);
};

class Sand : public Solid
{
private:
    void onDown();
    void onLeft();
    void onRight();
public:
    Sand();

    bool canActive(Vect pos);
};

class Snow : public Solid
{
private:
    void onDown();
    void onLeft();
    void onRight();
public:
    Snow();

    bool canActive(Vect pos);
};

class Water : public Liquid
{
private:
    void onDown();
    void onLeft();
    void onRight();
public:
    Water();

    bool canActive(Vect pos);
};

class Lava : public Liquid
{
private:
    void onDown();
    void onLeft();
    void onRight();
public:
    Lava();

    bool canActive(Vect pos);
};

#endif // PIXNODE_H_INCLUDED

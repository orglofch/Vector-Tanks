#include "PixNode.h"
#include <cstdlib>
#include <gl/gl.h>
#include <iostream>

PixNode::PixNode()
{
    pos.x = pos.y = 0;
    color.red = color.green = color.blue = color.alpha = 0.0f;

    isActive = false;
    isPartOf = false;
} // end default constructor

void PixNode::setPos(Vect pos)
{
    this->pos = {(int)pos.x, (int)pos.y};
} // end setPos method

void PixNode::setCol(Color color)
{
    this->color.red = color.red;
    this->color.green = color.green;
    this->color.blue = color.blue;
    this->color.alpha = color.alpha;
} // end setCol method

void PixNode::setPixMap(PixMap *pixMap)
{
    this->pixMap = pixMap;
} // end setPixMap method

Color PixNode::getCol()
{
    return color;
} // end getCol method

Vect PixNode::getPos()
{
    return pos;
} // end getPos method

int PixNode::getType()
{
    return type;
} // end getType method

void PixNode::makeActive()
{
    isActive = true;
} // end turnOn method

void PixNode::makeInactive()
{
    isActive = false;
} // end turnOff method

bool PixNode::checkActive()
{
    return isActive;
} // end checkActive method

bool PixNode::checkLiquid()
{
    return isLiquid;
} // end checkLiquid method

void PixNode::draw()
{
    if ((pos.x > 0  && !pixMap->checkPixCollide({pos.x - 1, pos.y})) ||
        (pos.x < mapWidth - 1 && !pixMap->checkPixCollide({pos.x + 1, pos.y})) ||
        (pos.y > 0 && !pixMap->checkPixCollide({pos.x, pos.y - 1})) ||
        (pos.y < mapHeight - 1 && !pixMap->checkPixCollide({pos.x, pos.y + 1})))
    {
        /** Neon Effect Likely temporary
        for (int i = 0; i < 10; i += 2) {
            glColor4f(color.red, color.green, color.blue, 0.003f);
            glBegin(GL_QUADS);
                glVertex2f(pos.x - 0.5 - i, pos.y - 0.5 - i);
                glVertex2f(pos.x - 0.5 - i, pos.y + 0.5 + i);
                glVertex2f(pos.x + 0.5 + i, pos.y + 0.5 + i);
                glVertex2f(pos.x + 0.5 + i, pos.y - 0.5 - i);
            glEnd();
        } // end for (i) **/
    }
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    /** Add if enought resources available if (pixMap->getSlopeAngle(pos) < 0) {
        glColor4f(color.red - 0.1, color.green - 0.1, color.blue - 0.1, color.alpha); // set drawing color
    } else { **/
    glColor4f(color.red, color.green, color.blue, color.alpha);
    glBegin(GL_QUADS);
        glVertex2f(pos.x - 0.5, pos.y - 0.5);
        glVertex2f(pos.x - 0.5, pos.y + 0.5);
        glVertex2f(pos.x + 0.5, pos.y + 0.5);
        glVertex2f(pos.x + 0.5, pos.y - 0.5);
    glEnd();
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);
} // end draw method

void PixNode::step()
{
    if (canActive({pos.x, pos.y + 1})) {
        onDown();
    } else if (canActive({pos.x - 1, pos.y}) && canActive({pos.x + 1, pos.y})) {
        if (rand() % 2 == 0) {
            onLeft();
        } else {
            onRight();
        }
    } else if (canActive({pos.x - 1, pos.y})) {
        onLeft();
    } else if (canActive({pos.x + 1, pos.y})) {
        onRight();
    }
} // end step method

Liquid::Liquid() : PixNode()
{
    isLiquid = true;
} // end default contstructor

Vect Liquid::findActiveSwap()
{
    Vect swap(pos);
    bool findingSwap;
    moveDir prevMove = DOWN; /** possibly make more intuitive **/
    do {
        findingSwap = false;
        if (swap.y > 0 && pixMap->checkPixCollide({swap.x, swap.y - 1}) && !pixMap->checkPixActive({swap.x, swap.y - 1}) && pixMap->checkPixType({swap.x, swap.y - 1}, type)) {
            if (pixMap->checkPixActive({swap.x, swap.y - 1})) {
                findingSwap = false;
            } else {
                swap.y -= 1;
                prevMove = DOWN;
                findingSwap = true;
            }
        } else if (prevMove != LEFT && swap.x < mapWidth - 1 && pixMap->checkPixCollide({swap.x + 1, swap.y}) && !pixMap->checkPixActive({swap.x + 1, swap.y}) && pixMap->checkPixType({swap.x + 1, swap.y}, type)) {
            if (pixMap->checkPixActive({swap.x + 1, swap.y})) {
                findingSwap = false;
            } else {
                swap.x += 1;
                prevMove = RIGHT;
                findingSwap = true;
            }
        } else if (prevMove != RIGHT && swap.x > 0 && pixMap->checkPixCollide({swap.x - 1, swap.y}) && !pixMap->checkPixActive({swap.x - 1, swap.y}) && pixMap->checkPixType({swap.x - 1, swap.y}, type)) {
            if (pixMap->checkPixActive({swap.x - 1, swap.y})) {
                findingSwap = false;
            } else {
                swap.x -= 1;
                prevMove = LEFT;
                findingSwap = true;
            }
        }
    } while (findingSwap);
    return swap;
} // end findActiveSwap method

Solid::Solid() : PixNode()
{
    isLiquid = false;
} // end default constructor

Dirt::Dirt() : Solid()
{
    type = DIRT;
    color = {0.6f, 0.7f, 0.3f, 1.0f};
} // end default cosntructor

bool Dirt::canActive(Vect pos)
{
    return !pixMap->checkPixCollide(pos) || pixMap->checkPixLiquid(pos);
} // end canActive method

void Dirt::onDown()
{
    Vect firstSwappedTo, swappedFrom, swappedTo;
    swappedFrom = pos;
    firstSwappedTo = swappedTo = Vect((int)pos.x, (int)pos.y + 1);
    do {
        pixMap->swapNodes(swappedFrom, swappedTo);
        swappedTo = swappedFrom;
        swappedFrom = Vect(swappedFrom.x, swappedFrom.y - 1);
    } while (pixMap->checkPixCollide(swappedFrom) && !pixMap->checkPixLiquid(swappedFrom));
    pixMap->pushActivePixNode(firstSwappedTo);
} // end onDown method

void Dirt::onLeft()
{
} // end onLeft method

void Dirt::onRight()
{
} // end onRight method

Sand::Sand() : Solid()
{
    type = SAND;
    color = {0.6f, 0.9f, 0.6f, 1.0f};
} // end default cosntrutor

bool Sand::canActive(Vect pos)
{
    return !pixMap->checkPixCollide(pos) || pixMap->checkPixLiquid(pos);
} // end canActive method

void Sand::onDown()
{
    Vect firstSwappedTo, swappedFrom, swappedTo;
    swappedFrom = pos;
    firstSwappedTo = swappedTo = Vect((int)pos.x, (int)pos.y + 1);
    int numFall = rand() % 10 + 1;
    int count = 0;
    do {
        pixMap->swapNodes(swappedFrom, swappedTo);
        swappedTo = swappedFrom;
        swappedFrom = Vect(swappedFrom.x, swappedFrom.y - 1);
        count += 1;
    } while (pixMap->checkPixCollide(swappedFrom) && !pixMap->checkPixLiquid(swappedFrom) && count <= numFall);
    pixMap->pushActivePixNode(firstSwappedTo);
} // end onDown method

void Sand::onLeft()
{
    if(rand() % 2 == 0) {
        Vect swappedFrom, swappedTo;
        swappedFrom = pos;
        swappedTo = Vect(pos.x - 1, pos.y);
        pixMap->swapNodes(swappedFrom, swappedTo);
        pixMap->pushActivePixNode(swappedTo);
    }
} // end onLeft method

void Sand::onRight()
{
    if(rand() % 2 == 0) {
        Vect swappedFrom, swappedTo;
        swappedFrom = pos;
        swappedTo = Vect(pos.x + 1, pos.y);
        pixMap->swapNodes(swappedFrom, swappedTo);
        pixMap->pushActivePixNode(swappedTo);
    }
} // end onRight method

Snow::Snow() : Solid()
{
    type = SNOW;
    color = {1.0f, 1.0f, 1.0f, 1.0f};
} // end default constructor

bool Snow::canActive(Vect pos)
{
    return !pixMap->checkPixCollide(pos);
} // end canActive method

void Snow::onDown()
{
    Vect swappedFrom, swappedTo;
    if(rand() % 4 != 0) {
        swappedFrom = pos;
        swappedTo = Vect(pos.x, pos.y + 1);
        pixMap->swapNodes(swappedFrom, swappedTo);
        pixMap->pushActivePixNode(swappedTo);
    } else {
        pixMap->pushActivePixNode(pos);
    }
} // end onDown method

void Snow::onLeft()
{
    if(rand() % 2 == 0) {
        Vect swappedFrom, swappedTo;
        swappedFrom = pos;
        swappedTo = Vect(pos.x - 1, pos.y);
        pixMap->swapNodes(swappedFrom, swappedTo);
        pixMap->pushActivePixNode(swappedTo);
    }
} // end onLeft method

void Snow::onRight()
{
    if(rand() % 2 == 0) {
        Vect swappedFrom, swappedTo;
        swappedFrom = pos;
        swappedTo = Vect(pos.x + 1, pos.y);
        pixMap->swapNodes(swappedFrom, swappedTo);
        pixMap->pushActivePixNode(swappedTo);
    }
} // end onRight method

Water::Water() : Liquid()
{
    type = WATER;
    color = {0.4f, 0.3f, 0.7f, 0.6f};
} // end default constructor

bool Water::canActive(Vect pos)
{
    return !pixMap->checkPixCollide(pos);
} // end canActive method

void Water::onDown()
{
    Vect swappedFrom, swappedTo;
    swappedFrom = findActiveSwap();
    swappedTo = Vect(pos.x, pos.y + 1);
    pixMap->swapNodes(swappedFrom, swappedTo);
    pixMap->pushActivePixNode(swappedTo);
} // end onDown method

void Water::onLeft()
{
    Vect swappedFrom, swappedTo;
    swappedFrom = findActiveSwap();
    swappedTo = Vect(pos.x - 1, pos.y);
    pixMap->swapNodes(swappedFrom, swappedTo);
    pixMap->pushActivePixNode(swappedTo);
} // end onLeft method

void Water::onRight()
{
    Vect swappedFrom, swappedTo;
    swappedFrom = findActiveSwap();
    swappedTo = Vect(pos.x + 1, pos.y);
    pixMap->swapNodes(swappedFrom, swappedTo);
    pixMap->pushActivePixNode(swappedTo);
} // end onRight method

Lava::Lava() : Liquid()
{
    type = LAVA;
    color = {0.9f, 0.3f, 0.2f, 1.0f};
} // end default constructor

bool Lava::canActive(Vect pos)
{
    return !pixMap->checkPixCollide(pos);
} // end canActive method

void Lava::onDown()
{
    Vect swappedFrom, swappedTo;
    swappedFrom = findActiveSwap();
    swappedTo = Vect(pos.x, pos.y + 1);
    pixMap->swapNodes(swappedFrom, swappedTo);
    pixMap->pushActivePixNode(swappedTo);
} // end onDown method

void Lava::onLeft()
{
    if(rand() % 6 != 0) {
        Vect swappedFrom, swappedTo;
        swappedFrom = findActiveSwap();
        swappedTo = Vect(pos.x - 1, pos.y);
        pixMap->swapNodes(swappedFrom, swappedTo);
        pixMap->pushActivePixNode(swappedTo);
    }
} // end onLeft method

void Lava::onRight()
{
    if(rand() % 6 != 0) {
        Vect swappedFrom, swappedTo;
        swappedFrom = findActiveSwap();
        swappedTo = Vect(pos.x + 1, pos.y);
        pixMap->swapNodes(swappedFrom, swappedTo);
        pixMap->pushActivePixNode(swappedTo);
    }
} // end onRight method

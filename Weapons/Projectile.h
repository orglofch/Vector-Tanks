/**
*
* @author: Owen Glofcheski
* @date: February 5, 2012
*
*/

#ifndef PROJECTILE_H_INCLUDED
#define PROJECTILE_H_INCLUDED

#include <gl/gl.h>
#include <cstdlib>

#include "../PixMap.h"
#include "../Globals.h"
#include "../Effects.h"
#include "../Color.h"
#include "../Math.h"
#include "../Vect.h"

class Projectile
{
private:
protected:
    PixMap *pixMap;

    float headSize;
    float energyCost;
    float damage;
    float blastRadius;
    float lightRadius;
    int level;
    float numMoveSplit; // Number of smaller movements a projectile movement is split into

    float gravity;
    float drag;
    float restitution;

    Vect pos;
    Vect velo;

    Color color;
    int flightCounter;

    Vect rayCollide();
    void stepMovement();
    virtual void createParticles();
public:
    Projectile(PixMap *pixMap, Vect pos, Vect velo, int level);

    // Setters
    void setBlastRadius(int blastRadius);
    void setLightRadius(int lightRadius);
    void setColor(Color color);
    void setGravity(Vect gravity); /** not done **/

    // Utility
    float getEnergyCost();
    void applyForce(Vect force);

    // Base
    void draw(); /** Possibly make virtual **/
    bool step();

    // Events
    virtual void onDie();
    virtual bool onActivate();
    virtual bool onPixCollide();
    virtual bool onPlayerCollide();
    virtual bool onBoundCollide();
    virtual bool onNoCollide();
};

#endif // PROJECTILE_H_INCLUDED

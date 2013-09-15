/**
*
* @author: Owen Glofcheski
* @date: February 14, 2012
*
*/

#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>

#include <gl/gl.h>
#include "../glfont.h"

#include "../Color.h"
#include "../Globals.h"
#include "../Math.h"
#include "../Vect.h"

/** Add sparking particle **/

class PixMap;

class Particle
{
protected:
    PixMap *pixMap;

    float gravity;
    float restitution; // How much is bounces when it collides with stuff
    float dispersionRate;
    float drag;

    Vect pos;
    Vect velo;
    float depth;

    float fadeInc; // The increment to which the particle is faded out

    Color color;

    int age;
    int duration;
    float size;

    Vect rayCollide();

    bool hasGravity; // Whether the particle is affected by gravity
    bool doesCollide; // Whether the particle does collide with objects
public:
    Particle(PixMap *pixMap, Vect pos, Vect velo, int duration, bool hasGravity, bool doesCollide);

    void setColor(Color color);
    void setGravity(float gravity);
    void setDrag(float drag);
    void setDispersion(float rate);

    virtual void draw() = 0;
    bool step();
    virtual bool stepPart() = 0;

    void applyForce(Vect force);
};

class LineParticle : public Particle
{
public:
    LineParticle(PixMap *pixMap, Vect pos, Vect velo, int duration, bool hasGravity, bool doesCollide);

    bool stepPart();
    void draw();
};

class CircleParticle : public Particle
{
public:
    CircleParticle(PixMap *pixMap, Vect pos, Vect velo, int duration, bool hasGravity, bool doesCollide);

    bool stepPart();
    void draw();
};

class AcceleratorLine : public Particle
{
private:
    Vect *accPos;
public:
    AcceleratorLine(PixMap *pixMap, Vect pos, Vect velo, Vect *accPos, int duration, bool doesCollide);

    bool stepPart();
    void draw();
};

class ElectricParticle : public Particle
{
private:
    int numBreaks;
    std::vector <Vect> arcBreaks;
public:
    ElectricParticle(PixMap *pixMap, Vect pos, Vect velo, int duration, bool doesCollide);

    bool stepPart();
    void draw();
};

class SmokeParticle : public Particle
{
private:
public:
    SmokeParticle(PixMap *pixMap, Vect pos, Vect velo, int duration, bool hasGravity, bool doesCollide);

    bool stepPart();
    void draw();
};

class DamageParticle : public Particle
{
private:
    GLuint damageTexture;
    PixelPerfectGLFont damageFont;

    std::string damage;
public:
    DamageParticle(PixMap *pixMap, Vect pos, Vect velo, std::string damage, int duration, bool hasGravity, bool doesCollide);

    bool stepPart();
    void draw();
};

class PixNodeParticle : public Particle
{
private:
    int type;

    Vect endPos;
public:
    PixNodeParticle(PixMap *pixMap, int type, Vect pos, Vect velo, Vect endPos, bool hasGravity, bool doesCollide);

    bool stepPart();
    void draw();
};

#endif // PARTICLE_H_INCLUDED

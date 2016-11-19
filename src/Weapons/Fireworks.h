/**
*
* @author: Owen Glofcheski
* @date: March 26, 2012
*
*/

#ifndef FIREWORKS_H_INCLUDED
#define FIREWORKS_H_INCLUDED

#include "Projectile.h"

class Fireworks : public Projectile
{
private:
    int numSpawn;

    void createParticles();
public:
    Fireworks(PixMap *pixMap, Vect pos, Vect velo, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

class FireworksProj : public Projectile
{
private:
    void createParticles();
public:
    FireworksProj(PixMap *pixMap, Vect pos, Vect velo, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

#endif // FIREWORKS_H_INCLUDED

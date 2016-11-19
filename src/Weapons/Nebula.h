/**
*
* @author: Owen Glofcheski
* @date: March 19, 2012
*
*/

#ifndef NEBULA_H_INCLUDED
#define NEBULA_H_INCLUDED

#include "Projectile.h"

class Nebula : public Projectile
{
private:
    int numSpawn;

    void createParticles();
public:
    Nebula(PixMap *pixMap, Vect pos, Vect velo, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

class Seeker : public Projectile
{
private:
    Vect home;

    void createParticles();
public:
    Seeker(PixMap *pixMap, Vect pos, Vect velo, Vect home, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

#endif // NEBULA_H_INCLUDED

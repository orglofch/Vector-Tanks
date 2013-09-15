/**
*
* @author: Owen Glofcheski
* @date: March 19, 2012
*
*/

#ifndef UNDERMINE_H_INCLUDED
#define UNDERMINE_H_INCLUDED

#include "Projectile.h"

class Undermine : public Projectile
{
private:
    int numSpawn;

    void createParticles();
public:
    Undermine(PixMap *pixMap, Vect pos, Vect velo, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

class UndermineProj : public Projectile
{
private:
    void createParticles();
public:
    UndermineProj(PixMap *pixMap, Vect pos, Vect velo, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

#endif // UNDERMINE_H_INCLUDED

/**
*
* @author: Owen Glofcheski
* @date: March 19, 2012
*
*/

#ifndef SPLITTER_H_INCLUDED
#define SPLITTER_H_INCLUDED

#include "Projectile.h"

class Splitter : public Projectile
{
private:
    int bounceCount;
    int numBounces;

    void createParticles();
public:
    Splitter(PixMap *pixMap, Vect pos, Vect velo, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

#endif // SPLITTER_H_INCLUDED

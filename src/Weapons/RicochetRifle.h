/**
*
* @author: Owen Glofcheski
* @date: March 23, 2012
*
*/

#ifndef RICOCHETRIFLE_H_INCLUDED
#define RICOCHETRIFLE_H_INCLUDED

#include "Projectile.h"

class RicochetRifle : public Projectile
{
private:
    int bounceCount;
    int numBounces;

    void createParticles();
public:
    RicochetRifle(PixMap *pixMap, Vect pos, Vect velo, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

#endif // RICOCHETRIFLE_H_INCLUDED

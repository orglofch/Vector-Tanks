/**
*
* @author: Owen Glofcheski
* @date: March 19, 2012
*
*/

#ifndef BURROWER_H_INCLUDED
#define BURROWER_H_INCLUDED

#include "Projectile.h"

class Burrower : public Projectile
{
private:
    int burrowDuration;
    int burrowTime;

    float drag;

    void createParticles();
public:
    Burrower(PixMap *pixMap, Vect pos, Vect velo, int burrowDuration, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

#endif // BURROWER_H_INCLUDED

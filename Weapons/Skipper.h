/**
*
* @author: Owen Glofcheski
* @date: March 27, 2012
*
*/


#ifndef SKIPPER_H_INCLUDED
#define SKIPPER_H_INCLUDED

#include "Projectile.h"

class Skipper : public Projectile
{
private:
    int skipCount;
    int numSkips;

    void createParticles();
public:
    Skipper(PixMap *pixMap, Vect pos, Vect velo, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

#endif // SKIPPER_H_INCLUDED

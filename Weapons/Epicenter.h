/**
*
* @author: Owen Glofcheski
* @date: April 6, 2012
*
*/

#ifndef EPICENTER_H_INCLUDED
#define EPICENTER_H_INCLUDED

#include "Projectile.h"

class Epicenter : public Projectile
{
private:
    int currentBlastRadius;

    int numPulses;
    int maxPulses;
    int currentSteps;
    int stepsBetweenPulses;

    void createParticles();
public:
    Epicenter(PixMap *pixMap, Vect pos, Vect velo, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

#endif // EPICENTER_H_INCLUDED

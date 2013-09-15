/**
*
* @author: Owen Glofcheski
* @date: March 20, 2012
*
*/

#ifndef PYROCLASM_H_INCLUDED
#define PYROCLASM_H_INCLUDED

#include "Projectile.h"

class Pyroclasm : public Projectile
{
private:
    void createParticles();
public:
    Pyroclasm(PixMap *pixMap, Vect pos, Vect velo, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

#endif // PYROCLASM_H_INCLUDED

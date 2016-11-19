/**
*
* @author: Owen Glofcheski
* @date: March 19, 2012
*
*/

#ifndef SHARPSHOT_H_INCLUDED
#define SHARPSHOT_H_INCLUDED

#include "Projectile.h"

class SharpShot : public Projectile
{
private:
    void createParticles();
public:
    SharpShot(PixMap *pixMap, Vect pos, Vect velo, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

#endif // SHARPSHOT_H_INCLUDED

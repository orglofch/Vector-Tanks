/**
*
* @author: Owen Glofcheski
* @date: March 26, 2012
*
*/

#ifndef BLACKSHEEPWALL_H_INCLUDED
#define BLACKSHEEPWALL_H_INCLUDED

#include "Projectile.h"

class BlackSheepWall : public Projectile
{
private:
    void createParticles();
public:
    BlackSheepWall(PixMap *pixMap, Vect pos, Vect velo, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

#endif // BLACKSHEEPWALL_H_INCLUDED

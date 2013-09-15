/**
*
* @author: Owen Glofcheski
* @date: March 24, 2012
*
*/

#ifndef CLUSTERBOMB_H_INCLUDED
#define CLUSTERBOMB_H_INCLUDED

#include "Projectile.h"

class ClusterBomb : public Projectile
{
private:
    int numSpawn;

    void createParticles();
public:
    ClusterBomb(PixMap *pixMap, Vect pos, Vect velo, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

#endif // CLUSTERBOMB_H_INCLUDED

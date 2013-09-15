/**
*
* @author: Owen Glofcheski
* @date: March 20, 2012
*
*/

#ifndef EMP_H_INCLUDED
#define EMP_H_INCLUDED

#include "Projectile.h"

class Emp : public Projectile
{
private:
    int endChargeTime; /** Remove if unused **/
    int currentChargeTime; /** Remove if unused **/

    void createParticles();
public:
    Emp(PixMap *pixMap, Vect pos, Vect velo, int level);

    // Events
    void onDie();
    bool onActivate();
    bool onPixCollide();
    bool onPlayerCollide();
    bool onBoundCollide();
    bool onNoCollide();
};

#endif // EMP_H_INCLUDED

#include "SharpShot.h"

SharpShot::SharpShot(PixMap *pixMap, Vect pos, Vect velo, int level) :
    Projectile(pixMap, pos, velo * 2, level)
{
    // Attributes
    damage = 90;
    energyCost = 90;

    color = {1.0f, 0.0f, 0.0f, 0.001f};
    numMoveSplit = 50; /** Possibly make based on velocity for all projectiles **/
    blastRadius = 0;
} // constructor

void SharpShot::createParticles()
{
    Particle *newParticle = new LineParticle(pixMap, pos, {-velo.x / 7, -velo.y / 7}, 1, true, false);
    newParticle->setColor(color);
    pEng.pushParticle(newParticle);
} // end createParticles method

void SharpShot::onDie()
{
    pixMap->checkPlayerDamages(pos, damage, blastRadius);
} // end onCollide method

bool SharpShot::onActivate()
{
    return false;
} // end onActive method

bool SharpShot::onPixCollide()
{
    pos = rayCollide();
    onDie();
    return true;
} // end onPixCollide method

bool SharpShot::onPlayerCollide()
{
    onDie();
    return true;
} // end onPlayerCollide method

bool SharpShot::onBoundCollide()
{
    onDie();
    return true;
} // end onBoundCollide method

bool SharpShot::onNoCollide()
{
    stepMovement();
    return false;
} // end onNoCollide method

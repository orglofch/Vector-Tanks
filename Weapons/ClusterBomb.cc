#include "ClusterBomb.h"

ClusterBomb::ClusterBomb(PixMap *pixMap, Vect pos, Vect velo, int level) : /** make it disable movement tracking etc when implemented **/
    Projectile (pixMap, pos, velo, level)
{
    // Attributes
    damage = 5;
    energyCost = 100;

    if (level == 3) {
        numSpawn = 10;
    } else if (level == 2) {
        numSpawn = 6;
    } else {
        numSpawn = 4;
    }
    color = {1.0f, 1.0f, 1.0f, 1.0f};
} // end constructor

void ClusterBomb::createParticles()
{
    Particle *newParticle = new LineParticle(pixMap, pos, {-velo.x / 5, -velo.y / 5}, 35, false, false);
    newParticle->setColor(color);
    pEng.pushParticle(newParticle);
} // end createParticles method

void ClusterBomb::onDie()
{
    pos.x -= velo.x;
    pos.y += velo.y;

    Projectile *newProjectile;
    for (int i = 0; i < numSpawn; i += 1) {
        float newVelo = (float)rand() / (float)RAND_MAX * 3 + 2;
        float angle = pixMap->getSlopeAngle(pos) + 50 + (float)rand() / (float)RAND_MAX * 80;
        newProjectile = new Projectile(pixMap, pos, createVect(-angle, newVelo), 1); /** createVect doesn't work **/
        newProjectile->setLightRadius(10);
        newProjectile->setBlastRadius(10);
        newProjectile->setColor(color);
        pixMap->pushProjectile(newProjectile);
    } // end for (i)
    mesh.applyForce(pos, blastRadius - 10, 0, 360);
    pixMap->pushRemove(pos, blastRadius);
    pixMap->checkPlayerDamages(pos, damage, blastRadius);
} // end onDie method

bool ClusterBomb::onActivate()
{
    onDie();
    return true;
} // end onActive method

bool ClusterBomb::onPixCollide()
{
    pos = rayCollide();
    onDie();
    return true;
} // end onPixCollide method

bool ClusterBomb::onPlayerCollide()
{
    onDie();
    return true;
} // end onPlayerCollide method

bool ClusterBomb::onBoundCollide()
{
    onDie();
    return true;
} // end onBoundCollide method

bool ClusterBomb::onNoCollide()
{
    stepMovement();
    return false;
} // end onNoCollide method

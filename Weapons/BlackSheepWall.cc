#include "BlackSheepWall.h"

BlackSheepWall::BlackSheepWall(PixMap *pixMap, Vect pos, Vect velo, int level) :
    Projectile (pixMap, pos, velo, level)
{
    // Attributes
    damage = 0;
    energyCost = 100;

    color = {1.0f, 1.0f, 1.0f, 1.0f};
    blastRadius = 0;
} // end constructor

void BlackSheepWall::createParticles()
{
    Particle *newParticle = new LineParticle(pixMap, pos, {-velo.x / 5, -velo.y / 5}, 5, false, false);
    newParticle->setColor(color);
    pEng.pushParticle(newParticle);
} // end createParticles method

void BlackSheepWall::onDie()
{
    ParticleEmitter *newEmitter = new ParticleEmitter(pixMap, 1);
    newEmitter->setPos(pos);
    newEmitter->setVelo({0.0f, 0.0f});
    newEmitter->setAngle(0, 360);
    newEmitter->setMaxParticles(10);
    newEmitter->setMaxParticleDuration(60);
    pEng.pushEmitter(newEmitter);

    pixMap->pushBuild(pos, 5, 100);
    pixMap->checkPlayerDamages(pos, damage, 30);
} // end onCollide method

bool BlackSheepWall::onActivate()
{
    return false;
} // end onActive method

bool BlackSheepWall::onPixCollide()
{
    pos = rayCollide();
    onDie();
    return true;
} // end onPixCollide method

bool BlackSheepWall::onPlayerCollide()
{
    onDie();
    return true;
} // end onPlayerCollide method

bool BlackSheepWall::onBoundCollide()
{
    onDie();
    return true;
} // end onBoundCollide method

bool BlackSheepWall::onNoCollide()
{
    stepMovement();
    return false;
} // end onNoCollide method

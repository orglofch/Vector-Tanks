#include "Fireworks.h"
#include <iostream>

Fireworks::Fireworks(PixMap *pixMap, Vect pos, Vect velo, int level) :
    Projectile (pixMap, pos, velo, level)
{
    // Attributes
    damage = 5;
    energyCost = 70;

    blastRadius = 5;
    if (level == 3) {
        numSpawn = 10;
    } else if (level == 2) {
        numSpawn = 15;
    } else {
        numSpawn = 12;
    }

    int randCol = rand() % 3;
    if (randCol == 0) {
        color = {1.0f, 0.2f, 0.2f, 1.0f};
    } else if (randCol == 1) {
        color = {0.2f, 1.0f, 0.2f, 1.0f};
    } else {
        color = {0.2f, 0.2f, 1.0f, 1.0f};
    }
} // end constructor

void Fireworks::createParticles()
{
    Particle *newParticle;
    if (level == 3) {
        newParticle = new LineParticle(pixMap, pos, -velo / 5, 2, true, false);
    } else {
        newParticle = new LineParticle(pixMap, pos, -velo / 5, 2, true, false);
    }
    newParticle->setColor(color);
    pEng.pushParticle(newParticle);
} // enc createParticles method

void Fireworks::onDie()
{
    ParticleEmitter *newEmitter = new ParticleEmitter(pixMap, 1);
    newEmitter->setPos(pos);
    newEmitter->setVelo({0.0f, 0.0f});
    newEmitter->setAngle(0, 360);
    newEmitter->setMaxParticles(10);
    newEmitter->setMaxParticleDuration(60);
    pEng.pushEmitter(newEmitter);

    pixMap->pushRemove(pos, blastRadius);
    pixMap->checkPlayerDamages(pos, damage, blastRadius);
} // end onDie method

bool Fireworks::onActivate()
{
    Projectile *newProjectile;
    for (int i = 0; i < numSpawn; i += 1) {
        float newVelo = (float)rand() / (float)RAND_MAX * 1.0f + 1.0f;
        float angle = (float)rand() / (float)RAND_MAX * 361;
        if (level == 3) {
            newProjectile = new Fireworks(pixMap, pos, createVect(angle, newVelo), 2);
            newProjectile->setLightRadius(40);
        } else {
            newProjectile = new FireworksProj(pixMap, pos, createVect(angle, newVelo), 1);
            newProjectile->setLightRadius(20);
        }
        newProjectile->setColor(color);
        pixMap->pushProjectile(newProjectile);
    } // end for (i)
    return true;
} // end onActive method

bool Fireworks::onPixCollide()
{
    pos = rayCollide();
    onDie();
    return true;
} // end onPixCollide method

bool Fireworks::onPlayerCollide()
{
    onDie();
    return true;
} // end onPlayerCollide method

bool Fireworks::onBoundCollide()
{
    onDie();
    return true;
} // end onBoundCollide method

bool Fireworks::onNoCollide()
{
    stepMovement();
    return false;
} // end onNoCollide method

FireworksProj::FireworksProj(PixMap *pixMap, Vect pos, Vect velo, int level) :
    Projectile (pixMap, pos, velo, level)
{
    // Attributes
    damage = 1;
    energyCost = 0;

    blastRadius = 1;
    if (level == 3) {
    } else if (level == 2) {
    } else {
    }
} // end constructor

void FireworksProj::createParticles()
{
    Particle *newParticle = new LineParticle(pixMap, pos, createVect(velo.getAngle() + 175 + (float)rand() / (float)RAND_MAX * 10, (float)rand() / (float)RAND_MAX * 2), 2, true, false);
    newParticle->setColor(color);
    pEng.pushParticle(newParticle);
} // enc createParticles method

void FireworksProj::onDie()
{
    pixMap->pushRemove(pos, blastRadius);
    pixMap->checkPlayerDamages(pos, damage, blastRadius);
} // end onDie method

bool FireworksProj::onActivate()
{
    return false;
} // end onActive method

bool FireworksProj::onPixCollide()
{
    pos = rayCollide();
    onDie();
    return true;
} // end onPixCollide method

bool FireworksProj::onPlayerCollide()
{
    onDie();
    return true;
} // end onPlayerCollide method

bool FireworksProj::onBoundCollide()
{
    onDie();
    return true;
} // end onBoundCollide method

bool FireworksProj::onNoCollide()
{
    stepMovement();
    return false;
} // end onNoCollide method


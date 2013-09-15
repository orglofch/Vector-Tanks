#include "Pyroclasm.h"

Pyroclasm::Pyroclasm(PixMap *pixMap, Vect pos, Vect velo, int level) : /** make it disable movement tracking etc when implemented **/
    Projectile (pixMap, pos, velo, level)
{
    // Attributes
    damage = 5;
    energyCost = 100;

    blastRadius = 20;
} // end constructor

void Pyroclasm::createParticles()
{
    float angle = velo.getAngle();
    ParticleEmitter *newEmitter = new FireEmitter(pixMap, 1);
    newEmitter->setPos(pos);
    newEmitter->setVelo({0.0f, 0.0f});
    newEmitter->setAngle(angle + 160, angle + 200);
    newEmitter->setMaxParticleVelo(2);
    newEmitter->setMaxParticles(5);
    newEmitter->setMaxParticleDuration(200);
    pEng.pushEmitter(newEmitter);
    Particle *newParticle = new ElectricParticle(pixMap, pos, velo / 3, 50, false);
    newParticle->setColor({0.8f, 0.3f, 0.3f, 1.0f});
    pEng.pushParticle(newParticle);
} // end createParticles method

void Pyroclasm::onDie()
{
    float angle = pixMap->getSlopeAngle(pos);
    ParticleEmitter *newEmitter = new FireEmitter(pixMap, 1);
    newEmitter->setPos(pos);
    newEmitter->setVelo({0.0f, 0.0f});
    newEmitter->setAngle(-(angle + 40), -(angle + 140)); /** Doesn't work correctly **/
    newEmitter->setMaxParticles(100);
    newEmitter->setMaxParticleDuration(60);
    pEng.pushEmitter(newEmitter);
    pixMap->pushRemove(pos, blastRadius);
    pixMap->checkPlayerDamages(pos, damage, blastRadius);
} // end onDie method

bool Pyroclasm::onActivate()
{
    return false;
} // end onActive method

bool Pyroclasm::onPixCollide()
{
    pos = rayCollide();
    onDie();
    return true;
} // end onPixCollide method

bool Pyroclasm::onPlayerCollide()
{
    onDie();
    return true;
} // end onPlayerCollide method

bool Pyroclasm::onBoundCollide()
{
    onDie();
    return true;
} // end onBoundCollide method

bool Pyroclasm::onNoCollide()
{
    stepMovement();
    return false;
} // end onNoCollide method


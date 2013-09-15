#include "Emp.h"

Emp::Emp(PixMap *pixMap, Vect pos, Vect velo, int level) : /** make it disable movement tracking etc when implemented **/
    Projectile (pixMap, pos, velo, level)
{
    // Attributes
    damage = 5;
    energyCost = 100;
} // end constructor

void Emp::createParticles()
{
    if (rand() % 3 == 0) {
        Vect newVelo = {(float)rand() / (float)RAND_MAX * 3, (float)rand() / (float)RAND_MAX * 3};
        Particle *newParticle = new ElectricParticle(pixMap, pos, newVelo, rand() % 60, false);
        newParticle->setColor({0.6f, 0.2f, 0.7f, 1.0f});
        pEng.pushParticle(newParticle);
    }
} // end createParticles method

void Emp::onDie()
{
    ParticleEmitter *newEmitter = new ElectricEmitter(pixMap, 20);
    newEmitter->setPos(pos);
    newEmitter->setVelo({0.0f, 0.0f});
    newEmitter->setAngle(0, 360);
    newEmitter->setMaxParticles(100);
    newEmitter->setMaxParticleDuration(60);
    pEng.pushEmitter(newEmitter);
    pixMap->checkPlayerDamages(pos, damage, blastRadius);
} // end onDie method

bool Emp::onActivate()
{
    return false;
} // end onActive method

bool Emp::onPixCollide()
{
    pos = rayCollide();
    onDie();
    return true;
} // end onPixCollide method

bool Emp::onPlayerCollide()
{
    onDie();
    return true;
} // end onPlayerCollide method

bool Emp::onBoundCollide()
{
    onDie();
    return true;
} // end onBoundCollide method

bool Emp::onNoCollide()
{
    stepMovement();
    return false;
} // end onNoCollide method

#include "Skipper.h"

Skipper::Skipper(PixMap *pixMap, Vect pos, Vect velo, int level) :
    Projectile (pixMap, pos, velo, level)
{
    // Attributes
    damage = 5;
    energyCost = 70;

    skipCount = 0;
    blastRadius = 15;
    if (level == 3) {
        numSkips = 5;
    } else if (level == 2) {
        numSkips = 3;
    } else {
        numSkips = 2;
    }

    color = {0.8f, 0.8f, 0.2f, 1.0f};
} // end constructor

void Skipper::createParticles()
{
    Particle *newParticle;
    newParticle = new LineParticle(pixMap, pos, {0, 0}, 100, true, false);
    newParticle->setColor(color);
    pEng.pushParticle(newParticle);
} // enc createParticles method

void Skipper::onDie()
{
    ParticleEmitter *newEmitter = new ParticleEmitter(pixMap, 1);
    newEmitter->setPos(pos);
    newEmitter->setVelo({0.0f, 0.0f});
    newEmitter->setAngle(0, 360);
    newEmitter->setMaxParticles(10);
    newEmitter->setMaxParticleDuration(60);
    pEng.pushEmitter(newEmitter);

    mesh.applyForce(pos, blastRadius, 0, 360);
    pixMap->pushRemove(pos, blastRadius);
    pixMap->checkPlayerDamages(pos, damage, blastRadius);
} // end onDie method

bool Skipper::onActivate()
{
    skipCount += 1;

    if (skipCount <= numSkips){
        velo.y = fabs(velo.y);
    }
    return false;
} // end onActive method

bool Skipper::onPixCollide()
{
    pos = rayCollide();
    onDie();
    return true;
} // end onPixCollide method

bool Skipper::onPlayerCollide()
{
    return onPixCollide();
} // end onPlayerCollide method

bool Skipper::onBoundCollide()
{
    return onPixCollide();
} // end onBoundCollide method

bool Skipper::onNoCollide()
{
    stepMovement();
    return false;
} // end onNoCollide method

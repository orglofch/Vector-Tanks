#include "Burrower.h"

Burrower::Burrower(PixMap *pixMap, Vect pos, Vect velo, int burrowDuration, int level) :
    Projectile (pixMap, pos, velo, level)
{
    // Attributes
    damage = 30;
    energyCost = 60;

    this->burrowDuration = burrowDuration;
    burrowTime = 0;
    drag = 0.92;
    if (level == 3) {
        blastRadius = 30;
    } else if (level == 2) {
        blastRadius = 25;
    } else {
        blastRadius = 15;
    }
    numMoveSplit = 2;
} // end constructor

void Burrower::createParticles()
{
    Particle *newParticle;
    if (pixMap->checkPixCollide(pos)){
        newParticle = new CircleParticle(pixMap, pos, {0, 0}, 100, false, false);
        newParticle->setColor({1.0f, 0.2f, 0.2f, 1.0f});
    } else {
        newParticle = new LineParticle(pixMap, pos, createVect(velo.getAngle() + 180, (float)rand() / (float)RAND_MAX * 2), 20, true, false);
        newParticle->setColor({0.8f, 0.3f, 0.3f, 1.0f});
    }
    pEng.pushParticle(newParticle);
} // end createParticles method

void Burrower::onDie()
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
    if(level == 3) {
        for (int i = 0; i < 10; i += 1)
        {
            float velo = 4; /** all other rands should be like this **/
            float angle = (float)rand() / (float)RAND_MAX * 361; /** all other rands should be like this **/
            Projectile *newProjectile = new Burrower(pixMap, pos, createVect(angle, velo), 100, 1);
            newProjectile->setBlastRadius(10);
            pixMap->pushProjectile(newProjectile);
        } // end for (i)
    }
    pixMap->checkPlayerDamages(pos, damage, blastRadius);
} // end onDie method

bool Burrower::onActivate()
{
    return false;
} // end onActive method

bool Burrower::onPixCollide()
{
    if (burrowTime < burrowDuration) {
        velo *= drag;

        burrowTime += 1;
        stepMovement();
        velo.y -= gravity;
        return false;
    } else {
        onDie();
        return true;
    }
} // end onPixCollide method

bool Burrower::onPlayerCollide()
{
    onDie();
    return true;
} // end onPlayerCollide method

bool Burrower::onBoundCollide()
{
    onDie();
    return true;
} // end onBoundCollide method

bool Burrower::onNoCollide()
{
    stepMovement();
    return false;
} // end onNoCollide method

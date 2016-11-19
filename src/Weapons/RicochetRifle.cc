#include "RicochetRifle.h"

RicochetRifle::RicochetRifle(PixMap *pixMap, Vect pos, Vect velo, int level) :
    Projectile (pixMap, pos, velo, level)
{
    // Attributes
    damage = 5;
    energyCost = 70;

    bounceCount = 0;
    blastRadius = 15;
    if (level == 3) {
        numBounces = 10;
    } else if (level == 2) {
        numBounces = 6;
    } else {
        numBounces = 5;
    }

    color = {0.8f, 0.8f, 0.2f, 1.0f};
} // end constructor

void RicochetRifle::createParticles()
{
    Particle *newParticle;
    newParticle = new LineParticle(pixMap, pos, Vect(0, 0), 100, true, false);
    newParticle->setColor(color);
    pEng.pushParticle(newParticle);
} // enc createParticles method

void RicochetRifle::onDie()
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

bool RicochetRifle::onActivate()
{
    return false;
} // end onActive method

bool RicochetRifle::onPixCollide()
{
    if (bounceCount < numBounces) {
        bounceCount += 1;

        mesh.applyForce(pos, blastRadius, 0, 360);
        pixMap->pushRemove(pos, blastRadius);
        pixMap->checkPlayerDamages(pos, damage, blastRadius);

        pos = rayCollide();

        pos.x -= velo.x;
        pos.y += velo.y;

        float totVelo = velo.getTotalVelo() * restitution;
        float angle = degreetoRad(pixMap->getSlopeAngle(pos) + 90); /** Not correct **/
        do {
            velo.x += cos(angle) / 2;
            velo.y += sin(angle) / 2;
        } while (velo.getTotalVelo() <= totVelo);
        return false;
    } else {
        onDie();
        return true;
    }
} // end onPixCollide method

bool RicochetRifle::onPlayerCollide()
{
    return onPixCollide();
} // end onPlayerCollide method

bool RicochetRifle::onBoundCollide()
{
    return onPixCollide();
} // end onBoundCollide method

bool RicochetRifle::onNoCollide()
{
    stepMovement();
    return false;
} // end onNoCollide method

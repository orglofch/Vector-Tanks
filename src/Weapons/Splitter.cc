#include "Splitter.h"

Splitter::Splitter(PixMap *pixMap, Vect pos, Vect velo, int level) :
    Projectile (pixMap, pos, velo, level)
{
    // Attributes
    damage = 0;
    energyCost = 70;

    bounceCount = 0;
    blastRadius = 15;
    if (level == 3) {
        numBounces = 4;
    } else if (level == 2) {
        numBounces = 5;
    } else {
        numBounces = 5;
    }
    restitution = 0.9;
} // end constructor

void Splitter::createParticles()
{
    Particle *newParticle;
    if (level == 3) {
        newParticle = new LineParticle(pixMap, pos, {-velo.x / 5, -velo.y / 5}, 85, false, false);
    } else if (level == 2) {
        newParticle = new LineParticle(pixMap, pos, {-velo.x / 5, -velo.y / 5}, 45, false, false);
    } else {
        newParticle = new LineParticle(pixMap, pos, {-velo.x / 5, -velo.y / 5}, 35, false, false);
    }
    newParticle->setColor(color);
    pEng.pushParticle(newParticle);
} // enc createParticles method

void Splitter::onDie()
{
    ParticleEmitter *newEmitter = new ParticleEmitter(pixMap, 1); /** Temporary duration **/
    newEmitter->setPos(pos);
    newEmitter->setVelo({0.0f, 0.0f}); /** Temporary **/
    newEmitter->setAngle(0, 360); /** Temporary Angles **/
    newEmitter->setMaxParticles(10);  /** Temporary Max Particles **/
    newEmitter->setMaxParticleDuration(60); /** Temporary maxDuration **/
    pEng.pushEmitter(newEmitter); /** Temporary **/
    mesh.applyForce(pos, blastRadius, 0, 360); /** Make use of vector struct **/
    pixMap->pushRemove(pos, blastRadius);
    pixMap->checkPlayerDamages(pos, damage, blastRadius);
} // end onDie method

bool Splitter::onActivate()
{
    return false;
} // end onActive method

bool Splitter::onPixCollide()
{
    if (bounceCount < numBounces) {
        bounceCount += 1;

        pos = rayCollide();

        pos.x -= velo.x;
        pos.y += velo.y;

        mesh.applyForce(pos, blastRadius - 10, 0, 360);

        float totVelo = velo.getTotalVelo() * restitution;
        float angle = degreetoRad(pixMap->getSlopeAngle(pos) + 90);
        do {
            velo.x += cos(angle) / 2;
            velo.y += sin(angle) / 2;
        } while (velo.getTotalVelo() <= totVelo);
        Projectile *newProjectile;
        if (level == 3) {
            for (int i = 0; i < 2; i += 1) {
                angle = radtoDegree(angle) + rand() % 30 - 15;
                newProjectile = new Splitter(pixMap, pos, {cos(degreetoRad(angle)) * 4, sin(degreetoRad(angle)) * 4}, 2);
            } // end for (i)
        } else if (level == 2) {
            for (int i = 0; i < 2; i += 1) {
                angle = radtoDegree(angle) + rand() % 30 - 15;
                newProjectile = new Projectile(pixMap, pos, {cos(degreetoRad(angle)) * 4, sin(degreetoRad(angle)) * 4}, 1);
            } // end for (i)
        } else {
            angle = radtoDegree(angle) + rand() % 30 - 15;
            newProjectile = new Projectile(pixMap, pos, {cos(degreetoRad(angle)) * 4, sin(degreetoRad(angle)) * 4}, 1);
        }
        newProjectile->setColor(color);
        pixMap->pushProjectile(newProjectile);
        return false;
    } else {
        onDie();
        return true;
    }
} // end onPixCollide method

bool Splitter::onPlayerCollide()
{
    onDie();
    return true;
} // end onPlayerCollide method

bool Splitter::onBoundCollide()
{
    return onPixCollide();
} // end onBoundCollide method

bool Splitter::onNoCollide()
{
    stepMovement();
    return false;
} // end onNoCollide method

#include "Undermine.h"

Undermine::Undermine(PixMap *pixMap, Vect pos, Vect velo, int level) :
    Projectile (pixMap, pos, velo, level)
{
    color = {0.9f, 0.9f, 0.1f, 0.8f};
    if (level == 3) {
        numSpawn = 10;
    } else if (level == 2) {
        numSpawn = 5;
    } else {
        numSpawn = 2;
    }
} // end constructor

void Undermine::createParticles()
{
    Particle *newParticle = new LineParticle(pixMap, pos, createVect(velo.getAngle() + 175 + (float)rand() / (float)RAND_MAX * 10, (float)rand() / (float)RAND_MAX * 2), 20, true, false);
    newParticle->setColor({0.8f, 0.8f, 0.3f, 1.0f});
    pEng.pushParticle(newParticle);
} // end createParticles method

void Undermine::onDie()
{
    pos.x += velo.x;
    pos.y -= velo.y;

    float angle = velo.getAngle();
    float totVelo = velo.getTotalVelo();
    for (int i = 0; i < numSpawn; i += 1)
    {
        velo = createVect(angle - (numSpawn / 2 + i) * 2, totVelo);
        Projectile *newProjectile = new UndermineProj(pixMap, pos, velo, level);
        newProjectile->setColor(color);
        newProjectile->setLightRadius(10);
        pixMap->pushProjectile(newProjectile);
    } // end for (i)
} // end onDie method

bool Undermine::onActivate()
{
    return false;
} // end onActive method

bool Undermine::onPixCollide()
{
    onDie();
    return true;
} // end onPixCollide method

bool Undermine::onPlayerCollide()
{
    return true;
} // end onPlayerCollide method

bool Undermine::onBoundCollide()
{
    onDie();
    return true;
} // end onBoundCollide method

bool Undermine::onNoCollide()
{
    stepMovement();
    return false;
} // end onNoCollide method

UndermineProj::UndermineProj(PixMap *pixMap, Vect pos, Vect velo, int level) :
    Projectile (pixMap, pos, velo, level)
{
    gravity = 0.2f;
    drag = 0.99f;

    if (level == 3) {
        blastRadius = 15;
        damage = 5;
    } else if (level == 2) {
        blastRadius = 9;
        damage = 8;
    } else {
        blastRadius = 5;
        damage = 10;
    }
} // end constructor

void UndermineProj::createParticles()
{
    Particle *newParticle = new LineParticle(pixMap, pos, {-velo.x / 5, -velo.y / 5}, 5, true, false);
    newParticle->setColor(color);
    pEng.pushParticle(newParticle);
} // end createParticles method

void UndermineProj::onDie()
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

bool UndermineProj::onActivate()
{
    return false;
} // end onActive method

bool UndermineProj::onPixCollide()
{
    velo *= drag;
    stepMovement();

    Particle *newParticle = new CircleParticle(pixMap, pos, {0, 0}, 40, false, false);
    newParticle->setColor({0.6f, 0.2f, 0.7f, 1.0f});
    pEng.pushParticle(newParticle);
    return false;
} // end onPixCollide method

bool UndermineProj::onPlayerCollide()
{
    onDie();
    return true;
} // end onPlayerCollide method

bool UndermineProj::onBoundCollide()
{
    onDie();
    return true;
} // end onBoundCollide method

bool UndermineProj::onNoCollide()
{
    onDie();
    return true;
} // end onNoCollide method

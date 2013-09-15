#include "Nebula.h"

Nebula::Nebula(PixMap *pixMap, Vect pos, Vect velo, int level) :
    Projectile (pixMap, pos, velo, level)
{
    // Attributes
    damage = 10;
    energyCost = 100;

    if (level == 3) {
        numSpawn = 10;
    } else if (level == 2) {
        numSpawn = 15;
    } else {
        numSpawn = 8;
    }
    blastRadius = 15;
} // end constructor

void Nebula::createParticles()
{
    float angle = velo.getAngle();
    float totVelo = velo.getTotalVelo();
    Particle *newParticle = new LineParticle(pixMap, pos, createVect(angle + (float)rand() / (float)RAND_MAX * 10 - 5, totVelo / 5), 60, true, false);
    newParticle->setColor({0.6f, 0.2f, 0.7f, 1.0f});
    pEng.pushParticle(newParticle);
    /** angle = (float)rand() / (float)RAND_MAX * 361;
    float distance = (float)rand() / (float)RAND_MAX * 30;
    newParticle = new AcceleratorLine(pixMap, pos + createVect(angle, distance), Vect(0, 0), &pos, 120, true);
    newParticle->setColor({0.6f, 0.8f, 0.4f, 1.0f});
    pEng.pushParticle(newParticle); **/
} // end createParticles method

void Nebula::onDie()
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

    pos.x -= velo.x;
    pos.y += velo.y;

    for (int i = 0; i < numSpawn; i += 1)
    {
        float velo;
        float angle = pixMap->getSlopeAngle(pos) + 60 + (float)rand() / (float)RAND_MAX * 60;
        Projectile *newProjectile;
        if (level == 3) {
            velo = rand() % 5 + 2;
            newProjectile = new Nebula(pixMap, {pos.x, pos.y}, createVect(-angle, velo), 1);
        } else {
            velo = rand() % 15 + 4;
            newProjectile = new Seeker(pixMap, {pos.x, pos.y}, createVect(-angle, velo), {pos.x + rand() % 31 - 15, pos.y + rand() % 31 - 15}, level);
        }
        pixMap->pushProjectile(newProjectile); /** Temporary **/
    } // end for (i)
    pixMap->checkPlayerDamages(pos, damage, blastRadius);
} // end onDie method

bool Nebula::onActivate()
{
    return false;
} // end onActive method

bool Nebula::onPixCollide()
{
    pos = rayCollide();
    onDie();
    return true;
} // end onPixCollide method

bool Nebula::onPlayerCollide()
{
    onDie();
    return true;
} // end onPlayerCollide method

bool Nebula::onBoundCollide()
{
    onDie();
    return true;
} // end onBoundCollide method

bool Nebula::onNoCollide()
{
    stepMovement();
    return false;
} // end onNoCollide method

Seeker::Seeker(PixMap *pixMap, Vect pos, Vect velo, Vect home, int level) :
    Projectile (pixMap, pos, velo, level)
{
    // Attributes
    damage = 5;

    this->home = home;
    blastRadius = 5;
} // end constructor

void Seeker::createParticles()
{
    Particle *newParticle = new LineParticle(pixMap, pos, -velo / 5, 60, true, false);
    newParticle->setColor({0.6f, 0.2f, 0.7f, 1.0f});
    pEng.pushParticle(newParticle);
} // end createParticles method

void Seeker::onDie()
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

bool Seeker::onActivate()
{
    return false;
} // end onActive method

bool Seeker::onPixCollide()
{
    pos = rayCollide();
    onDie();
    return true;
} // end onPixCollide method

bool Seeker::onPlayerCollide()
{
    onDie();
    return true;
} // end onPlayerCollide method

bool Seeker::onBoundCollide()
{
    onDie();
    return true;
} // end onBoundCollide method

bool Seeker::onNoCollide()
{
    stepMovement();

    float angle = angleBetween(pos, home);
    Vect force = createVect(angle, 0.8);
    applyForce(force);

    if (distance(pos, home) <= 2 && flightCounter > 10) {
        onDie();
        return true;
    }
    return false;
} // end onNoCollide method

#include "Torrent.h"
#include "../PixNode.h"

Torrent::Torrent(PixMap *pixMap, Vect pos, Vect velo, int level) :
    Projectile (pixMap, pos, velo, level)
{
    // Attributes
    damage = 10;

    color = {0.4f, 0.3f, 0.9f, 1.0f};
} // end constructor

void Torrent::createParticles()
{
    if (rand() % 3 == 0) {
        Particle *newParticle = new LineParticle(pixMap, pos, {0.0f, -(float)rand() / (float)RAND_MAX}, 200, false, false);
        newParticle->setColor(color);
        pEng.pushParticle(newParticle);
    }
} // end createParticles method

void Torrent::onDie()
{
    ParticleEmitter *newEmitter = new ParticleEmitter(pixMap, 1); /** Temporary duration **/
    newEmitter->setPos(pos);
    newEmitter->setVelo({0.0f, 0.0f}); /** Temporary **/
    newEmitter->setAngle(0, 360); /** Temporary Angles **/
    newEmitter->setMaxParticles(10);  /** Temporary Max Particles **/
    newEmitter->setMaxParticleDuration(60); /** Temporary maxDuration **/
    pEng.pushEmitter(newEmitter); /** Temporary **/
    mesh.applyForce(pos, blastRadius, 0, 360); /** Make use of vector struct **/
    pixMap->pushLiquify(pos, blastRadius);
    pixMap->checkPlayerDamages(pos, damage, blastRadius);
} // end onDie method

bool Torrent::onActivate()
{
    return false;
} // end onActive method

bool Torrent::onPixCollide()
{
    pos = rayCollide();
    onDie();
    return true;
} // end onPixCollide method

bool Torrent::onPlayerCollide()
{
    onDie();
    return true;
} // end onPlayerCollide method

bool Torrent::onBoundCollide()
{
    onDie();
    return true;
} // end onBoundCollide method

bool Torrent::onNoCollide()
{
    pixMap->createPixNode(pos, WATER, {0.4f, 0.3f, 0.7f, 0.6f});
    stepMovement();
    return false;
} // end onNoCollide method

TorrentProj::TorrentProj(PixMap *pixMap, Vect pos, Vect velo, int level) :
    Projectile (pixMap, pos, velo, level)
{
    // Attributes
    damage = 5;

    color = {0.4f, 0.3f, 0.9f, 0.01f};

    blastRadius = 1;
    restitution = 0.2;
    numMoveSplit = 10;
} // end constructor

void TorrentProj::createParticles()
{
    Particle *newParticle = new LineParticle(pixMap, pos, {-velo.x / 5, -velo.y / 5}, 5, false, false);
    newParticle->setColor(color);
    pEng.pushParticle(newParticle);
} // end createParticles method

void TorrentProj::onDie()
{
    pixMap->pushLiquify(pos, blastRadius);
    pixMap->checkPlayerDamages(pos, damage, blastRadius);
} // end onDie method

bool TorrentProj::onActivate()
{
    return false;
} // end onActive method

bool TorrentProj::onPixCollide()
{
    pos = rayCollide();
    onDie();
    return true;
} // end onPixCollide method

bool TorrentProj::onPlayerCollide()
{
    onDie();
    return true;
} // end onPlayerCollide method

bool TorrentProj::onBoundCollide()
{
    onDie();
    return true;
} // end onBoundCollide method

bool TorrentProj::onNoCollide()
{
    stepMovement();
    return false;
} // end onNoCollide method

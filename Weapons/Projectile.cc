#include "Projectile.h"
#include <iostream>

Projectile::Projectile(PixMap *pixMap, Vect pos, Vect velo, int level)
{
    // Attributes
    damage = 10;
    energyCost = 50;
    lightRadius = 60;

    this->pixMap = pixMap;
    this->pos = pos;
    this->velo = velo;

    gravity = -0.1f;
    drag = 0.99;
    restitution = 1.0f;

    flightCounter = 0;

    color.red = 0.3f;
    color.green = 0.2f;
    color.blue = 0.7f;
    color.alpha = 1.0f;

    headSize = 1;

    this->level = level;
    if (level == 3) {
        blastRadius = 30;
    } else if (level == 2) {
        blastRadius = 15;
    } else {
        blastRadius = 8;
    }

    numMoveSplit = 5;
} // end constructor

Vect Projectile::rayCollide()
{
    Vect rayPos = pos;
    if (velo != Vect(0, 0) && flightCounter > 10) {
        float angle = velo.getAngle() - 180;
        Vect rayVelo = createVect(angle, 1);
        while(pixMap->checkPixCollide(rayPos) && !pixMap->checkInBounds(rayPos))
        {
            rayPos.x += rayVelo.x;
            rayPos.y -= rayVelo.y;
        } // end while
    }
    return rayPos;
} // end rayCollide method

void Projectile::stepMovement()
{
    flightCounter += 1;

    velo *= drag;

    velo.y += gravity;

    for (int i = 0; i < numMoveSplit; i += 1) {
        pos.x += velo.x / numMoveSplit;
        pos.y -= velo.y / numMoveSplit;

        createParticles();
    } // end for (i)
} // end stepMovement method

void Projectile::createParticles()
{
    Particle *newParticle = new LineParticle(pixMap, pos, createVect(velo.getAngle() + 180, (float)rand() / (float)RAND_MAX * 2), 2, true, false);
    newParticle->setColor({0.8f, 0.8f, 0.3f, 1.0f});
    pEng.pushParticle(newParticle);
    float angle = (float)rand() / (float)RAND_MAX * 40 + 70;
    float newVelo = (float)rand() / (float)RAND_MAX / 4;
    newParticle = new SmokeParticle(pixMap, pos, {cos(degreetoRad(angle)) * newVelo, sin(degreetoRad(angle)) * newVelo}, 300, true, false);
    newParticle->setColor({0.2f, 0.2f, 0.2f, 0.12f});
    pEng.pushParticle(newParticle);
} // end createParticles method

/** void Projectile::drawTrajectory()
{
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    Vect temp = pos;
    Vect tempVelo = velo;
    glBegin(GL_LINES);
        while(temp.x < mapWidth && temp.x >= 0 && temp.y >= 0 && temp.y < mapHeight && !pixMap->checkPixCollide(temp))
        {
            glVertex2f(temp.x, temp.y);
            glVertex2f(temp.x + tempVelo.x, temp.y - tempVelo.y);
            temp.x += tempVelo.x;
            temp.y -= tempVelo.y;
            tempVelo.x *= drag;
            tempVelo.y *= drag;
            tempVelo.y += gravity;
        } // end for (i)
    glEnd();
} // end drawTrajectory method **/

void Projectile::setBlastRadius(int blastRadius)
{
    this->blastRadius = blastRadius;
} // end setBlastRadius method

void Projectile::setLightRadius(int lightRadius)
{
    this->lightRadius = lightRadius;
} // end setLightRadius method

void Projectile::setColor(Color color)
{
    this->color = color;
} // end setColor method

void Projectile::setGravity(Vect gravity) /** Not done **/
{
    this->gravity = gravity.y; /** Make gravity Vect structure **/
} // end setGravity method

float Projectile::getEnergyCost()
{
    return energyCost;
} // end getEnergyCost method

void Projectile::applyForce(Vect force)
{
    velo.x += force.x;
    velo.y -= force.y;
} // end applyForce method

void Projectile::draw()
{
    if (pos.y + lightRadius >= 0) {
        glPointSize(headSize);
        glColor4f(color.red, color.green, color.blue, color.alpha); // set drawing color
        glBegin(GL_POINTS);
            glVertexVect(pos);
        glEnd();
        /** Bloom effect remove later **/
        for (int i = 0; i < lightRadius; i += 6) {
            glColor4f(color.red, color.green, color.blue, 0.015f); // set drawing color
            glBegin(GL_TRIANGLE_FAN);
                glVertex2f(pos.x, pos.y);
                for (int j = 0; j <= 360; j += 5)
                    glVertexVect(pos + createVect(j, i));
            glEnd();
        } // end for (i)
    } else {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
            glVertex2f(pos.x, 5); glVertex2f(pos.x, 10);
        glEnd();
    }
    /** drawTrajectory(); **/
} // end draw method

bool Projectile::step()
{
    if (pixMap->checkInBounds(pos)) {
        return onBoundCollide();
    } else if (pixMap->checkPixCollide(pos)){
        return onPixCollide();
    } else if (flightCounter > 10 && pixMap->checkPlayerCollide(pos)) {
        return onPlayerCollide();
    } else {
        return onNoCollide();
    }
    return false;
} // end step method


//********************************* Events ******************************************
void Projectile::onDie()
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
    pixMap->checkPlayerDamages(pos, damage, 30);
} // end onCollide method

bool Projectile::onActivate()
{
    return false;
} // end onActive method

bool Projectile::onPixCollide()
{
    pos = rayCollide();
    onDie();
    return true;
} // end onPixCollide method

bool Projectile::onPlayerCollide()
{
    onDie();
    return true;
} // end onPlayerCollide method

bool Projectile::onBoundCollide()
{
    onDie();
    return true;
} // end onBoundCollide method

bool Projectile::onNoCollide()
{
    stepMovement();
    return false;
} // end onNoCollide method

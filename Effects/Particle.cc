#include "Particle.h"
#include "../PixMap.h"

Particle::Particle(PixMap *pixMap, Vect pos, Vect velo, int duration, bool hasGravity, bool doesCollide)
{
    this->pixMap = pixMap;

    gravity = -0.01f;
    restitution = 0.6;
    dispersionRate = 0.00f;
    drag = 0.99f;

    this->pos = pos;
    this->velo = velo;

    float veloDepth = sqrt(pow(velo.x, 2) + pow(velo.y, 2));
    float maxDepth = (veloDepth - 1);
    depth = (float)rand() / (float)RAND_MAX * maxDepth - maxDepth / 2; /** Not finalized **/

    age = 0;
    this->duration = duration;
    size = 0.5f;

    color.red = color.green = color.blue = color.alpha = 0.0f;
    fadeInc = color.alpha / duration;

    this->hasGravity = hasGravity;
    this->doesCollide = doesCollide;
} // end constructor

Vect Particle::rayCollide()
{
    Vect rayPos = pos;
    if (velo != Vect(0, 0) && age > 10) {
        float angle = velo.getAngle() - 180;
        Vect rayVelo = createVect(angle, 1);
        while(pixMap->checkPixCollide(rayPos) && !pixMap->checkInBounds(rayPos))
        {
            rayPos.x += rayVelo.x;
            rayPos.y -= rayVelo.y;
        } // end while
    }
    velo.x = velo.y = 0;
    return rayPos;
} // end rayCollide method

void Particle::setColor(Color color)
{
    this->color = color;
    fadeInc = color.alpha / duration;
} // end setColor method

void Particle::setGravity(float gravity)
{
    this->gravity = gravity;
} // end setGravity method

void Particle::setDispersion(float rate)
{
    dispersionRate = rate;
} // end hasDispersion method

bool Particle::step()
{
    color.alpha -= fadeInc; // Fade out slowlys
    age += 1;
    if (age > duration && duration != -1)
        return true;
    pos.x += velo.x;
    pos.y -= velo.y;

    velo.x *= drag;
    velo.y *= drag;

    if (hasGravity)
        velo.y += gravity; /** Add drag possibly **/
    if (pos.x < 0) {
        pos.x = 0;
        velo.x *= -restitution;
    } else if (pos.x >= mapWidth) {
        pos.x = mapWidth - 1;
        velo.x *= -restitution;
    }
    if (pos.y >= mapHeight) {
        pos.y = mapHeight - 1;
        velo.y *= -restitution;
    }
    if (doesCollide && pixMap->checkPixCollide(pos))
        pos = rayCollide();

    return stepPart();
} // end step method

void Particle::applyForce(Vect force)
{
    velo.x = force.x;
    velo.y = force.y;
} // end applyForce method

LineParticle::LineParticle(PixMap *pixMap, Vect pos, Vect velo, int duration, bool hasGravity, bool doesCollide) :
        Particle(pixMap, pos, velo, duration, hasGravity, doesCollide)
{
} // end constructor

void LineParticle::draw()
{
    glColor4f(color.red + depth / 2, color.green + depth / 2, color.blue + depth / 2, color.alpha + depth / 2); /** Finalize depth usage **/
    glBegin(GL_LINES);
        glVertexVect(pos); glVertex2f(pos.x - velo.x * 5, pos.y + velo.y * 5);
    glEnd();
} // end draw method

bool LineParticle::stepPart()
{
    return false;
} // end step method

CircleParticle::CircleParticle(PixMap *pixMap, Vect pos, Vect velo, int duration, bool hasGravity, bool doesCollide) :
        Particle(pixMap, pos, velo, duration, hasGravity, doesCollide)
{
} // end constructor

bool CircleParticle::stepPart()
{
    return false;
} // ends step method

void CircleParticle::draw()
{
    glColor(color);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(pos.x, pos.y);
        for (int j = 0; j <= 360; j += 5)
            glVertex2f(pos.x + sin(degreetoRad(j)) * size, pos.y + cos(degreetoRad(j)) * size);
    glEnd();
    size += dispersionRate;
} // end draw method

AcceleratorLine::AcceleratorLine(PixMap *pixMap, Vect pos, Vect velo, Vect *accPos, int duration, bool doesCollide) :
        Particle(pixMap, pos, velo, duration, false, doesCollide)
{
    this->accPos = accPos;
} // end constructor

bool AcceleratorLine::stepPart()
{
    float angle = atan2(pos.y - accPos->y, accPos->x - pos.x);
    velo.x += cos(angle) / 50;
    velo.y += sin(angle) / 50;
    return false;
} // ends step method

void AcceleratorLine::draw()
{
    glColor4f(color.red + depth / 2, color.green + depth / 2, color.blue + depth / 2, color.alpha + depth / 2); /** Finalize depth usage **/
    glBegin(GL_LINES);
        glVertex2f(pos.x, pos.y); glVertex2f(pos.x - velo.x * 5, pos.y + velo.y * 5);
    glEnd();
} // end draw method

ElectricParticle::ElectricParticle(PixMap *pixMap, Vect pos, Vect velo, int duration, bool doesCollide) :
        Particle(pixMap, pos, velo, duration, false, doesCollide)
{
    numBreaks = rand() % 20 + 5;
    for(int i = 0; i < numBreaks; i += 1) {
        arcBreaks.push_back(Vect(pos));
    } // end for (i)
} // end constructor

bool ElectricParticle::stepPart()
{
    float totVelo = velo.getTotalVelo();
    float newAngle = velo.getAngle() - 180 + (float)rand() / (float)RAND_MAX * 361;
    velo = {cos(degreetoRad(newAngle)) * totVelo, sin(degreetoRad(newAngle)) * totVelo};
    for (int i = numBreaks - 1; i > 0; i -= 1) {
        arcBreaks[i] = arcBreaks [i - 1];
    } // end for (i)
    arcBreaks[0] = pos;
    return false;
} // ends step method

void ElectricParticle::draw()
{
    glColor4f(color.red + depth / 2, color.green + depth / 2, color.blue + depth / 2, color.alpha + depth / 2); /** Finalize depth usage **/
    glBegin(GL_LINES);
        glVertexVect(pos); glVertexVect(arcBreaks[0]);
    glEnd();
    for (int i = 0; i < numBreaks - 1; i += 1) {
        glBegin(GL_LINES);
            glVertexVect(arcBreaks[i]); glVertexVect(arcBreaks[i + 1]);
        glEnd();
    } // end for (i)
} // end draw method

SmokeParticle::SmokeParticle(PixMap *pixMap, Vect pos, Vect velo, int duration, bool hasGravity, bool doesCollide) :
        Particle(pixMap, pos, velo, duration, hasGravity, doesCollide)
{
    dispersionRate = 0.4f;
    gravity = 0.01f;
} // end constructor

bool SmokeParticle::stepPart()
{
    color.alpha -= fadeInc * (duration / 100);
    return false;
} // ends step method

void SmokeParticle::draw()
{
    glColor(color);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(pos.x, pos.y);
        for (int j = 0; j <= 360; j += 5)
            glVertex2f(pos.x + sin(degreetoRad(j)) * size, pos.y + cos(degreetoRad(j)) * size);
    glEnd();
    size += dispersionRate;
} // end draw method

DamageParticle::DamageParticle(PixMap *pixMap, Vect pos, Vect velo, std::string damage, int duration, bool hasGravity, bool doesCollide) :
        Particle(pixMap, pos, velo, duration, hasGravity, doesCollide)
{
    damageFont.Create("damage.glf", damageTexture);
    this->damage = damage;
    drag = 1.0f;
} // end constructor

bool DamageParticle::stepPart()
{
    return false;
} // ends step method

void DamageParticle::draw()
{
    glEnable(GL_TEXTURE_2D);
    glColor(color);
    damageFont.Begin();
        damageFont.textOut(damage, pos.x, pos.y, 0.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
} // end draw method

PixNodeParticle::PixNodeParticle(PixMap *pixMap, int type, Vect pos, Vect velo, Vect endPos, bool hasGravity, bool doesCollide) :
        Particle(pixMap, pos, velo, duration, hasGravity, doesCollide)
{
    duration = -1;

    this->type = type;
    this->endPos = endPos;
} // end constructor

bool PixNodeParticle::stepPart()
{
    float angle = atan2(pos.y - endPos.y, endPos.x - pos.x);
    velo.x += cos(angle) / 30;
    velo.y += sin(angle) / 30;
    if (distance(pos, endPos) <= 0.5) {
        pixMap->createPixNode(endPos, type, color);
        return true;
    }
    return false;
} // ends step method

void PixNodeParticle::draw()
{
    glColor4f(color.red + depth / 2, color.green + depth / 2, color.blue + depth / 2, color.alpha + depth / 2); /** Finalize depth usage **/
    glBegin(GL_LINES);
        glVertex2f(pos.x, pos.y); glVertex2f(pos.x - velo.x * 5, pos.y + velo.y * 5);
    glEnd();
} // end draw method

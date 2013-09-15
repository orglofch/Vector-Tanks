#include "Epicenter.h"

Epicenter::Epicenter(PixMap *pixMap, Vect pos, Vect velo, int level) : /** make it disable movement tracking etc when implemented **/
    Projectile (pixMap, pos, velo, level)
{
    // Attributes
    damage = 1;
    energyCost = 100;


    numPulses = 0;
    maxPulses = 5;
    stepsBetweenPulses = 35;
    currentSteps = stepsBetweenPulses;
    currentBlastRadius = 5;
} // end constructor

void Epicenter::createParticles()
{
    Particle *newParticle = new LineParticle(pixMap, pos, Vect(0, 0), 50, true, false);
    newParticle->setColor({0.2f, 0.9f, 0.3f, 1.0f});
    pEng.pushParticle(newParticle);
} // end createParticles method

void Epicenter::onDie()
{
} // end onDie method

bool Epicenter::onActivate()
{
    return false;
} // end onActive method

bool Epicenter::onPixCollide()
{
    pos = rayCollide();
    if (numPulses == 0)
        velo.x = velo.y = 0;
    if (numPulses <= maxPulses) {
        if (currentSteps >= stepsBetweenPulses) {
            mesh.applyForce(pos, currentBlastRadius / 2, 0, 360);
            pixMap->pushRemove(pos, currentBlastRadius);
            pixMap->checkPlayerDamages(pos, damage, currentBlastRadius);

            damage += 1;
            numPulses += 1;
            currentBlastRadius += 10;
            currentSteps = 0;
        } else {
            currentSteps += 1;
        }
        return false;
    }
    return true;
} // end onPixCollide method

bool Epicenter::onPlayerCollide()
{
    return onPixCollide();
} // end onPlayerCollide method

bool Epicenter::onBoundCollide()
{
    return onPixCollide();
} // end onBoundCollide method

bool Epicenter::onNoCollide()
{
    stepMovement();
    if (numPulses > 0)
        return onPixCollide();
    return false;
} // end onNoCollide method

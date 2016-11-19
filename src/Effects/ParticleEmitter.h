/**
*
* @author: Owen Glofcheski
* @date: February 14, 2012
*
*/

#ifndef PARTICLEEMITTER_H_INCLUDED
#define PARTICLEEMITTER_H_INCLUDED

#include <vector>

#include "Particle.h"
#include "../Math.h"
#include "../Vect.h"

class ParticleEmitter
{
protected:
    PixMap *pixMap;

    Vect pos;
    Vect velo;

    float drag;
    float gravity;

    int startAngle, endAngle; // Emits particles between these angles

    int age;
    int duration;
    int maxParticleDuration;
    float maxParticleVelo;

    unsigned int maxParticles;
    std::vector <Particle*> particles;

    void stepParticles();
    bool dieStep();
    virtual void aliveStep();
public:
    ParticleEmitter(PixMap *pixMap, int duration);

    void setPos(Vect pos);
    void setVelo(Vect velo);
    void setAngle(int startAngle, int endAngle);
    void setMaxParticles(int maxParticles);
    void setMaxParticleDuration(int maxParticleDuration);
    void setMaxParticleVelo(float velo);
    void setGravity(float gravity);

    void draw();
    bool step();
};

class FireEmitter : public ParticleEmitter
{
private:
    void aliveStep();
public:
    FireEmitter(PixMap *pixMap, int duration);
};

class ElectricEmitter : public ParticleEmitter
{
private:
    void aliveStep();
public:
    ElectricEmitter(PixMap *pixMap, int duration);
};

#endif // PARTICLEEMITTER_H_INCLUDED

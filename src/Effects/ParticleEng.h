/**
*
* @author: Owen Glofcheski
* @date: February 14, 2012
*
*/

#ifndef PARTICLEENG_H_INCLUDED
#define PARTICLEENG_H_INCLUDED

#include <cstdlib>
#include <vector>

#include "ParticleEmitter.h"
#include "../Vect.h"

class ParticleEng
{
private:
    std::vector <ParticleEmitter*> activeEmitters;
    std::vector <ParticleEmitter*> stackEmitters;
    std::vector <Particle*> activeParticles;
    std::vector <Particle*> stackParticles;

    void unloadParticleStack();
    void unloadEmitterStack();
public:
    void pushEmitter(ParticleEmitter *newEmitter);
    void pushParticle(Particle *newParticle);

    void draw();
    void step();
};

#endif // PARTICLEENG_H_INCLUDED

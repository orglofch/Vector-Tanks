#include "ParticleEng.h"
#include <iostream>

void ParticleEng::unloadParticleStack()
{
    for(std::vector<Particle*>::iterator i = stackParticles.begin(); i < stackParticles.end(); i += 1) {
        try {
            activeParticles.push_back(*i);
        } catch(...){
            std::cout << "Could not add new Particle to pEng active particles." << std::endl;
        }
    } // end for (i)
    stackParticles.clear();
} // end unloadParticleStack method

void ParticleEng::unloadEmitterStack()
{
    for(std::vector<ParticleEmitter*>::iterator i = stackEmitters.begin(); i < stackEmitters.end(); i += 1) {
        try {
            activeEmitters.push_back(*i);
        } catch(...) {
            std::cout << "Could not add new Emitter to pEng active emitters." << std::endl;
        }
    } // end for (i)
    stackEmitters.clear();
} // end unloadEmitterStack method

void ParticleEng::pushEmitter(ParticleEmitter *newEmitter)
{
    try {
        stackEmitters.push_back(newEmitter);
    } catch(...) {
        std::cout << "Could not add new Emitter to pEng stack." << std::endl;
    }
} // end pushEmitter method

void ParticleEng::pushParticle(Particle *newParticle)
{
    try {
        stackParticles.push_back(newParticle);
    } catch(...) {
        std::cout << "Could not add new Particle to pEng stack." << std::endl;
    }
} // end pushParticle method

void ParticleEng::draw()
{
    for (std::vector<ParticleEmitter*>::iterator i = activeEmitters.begin(); i < activeEmitters.end(); i += 1)
        (*i)->draw();
    for (std::vector<Particle*>::iterator i = activeParticles.begin(); i < activeParticles.end(); i += 1)
        (*i)->draw();
} // end draw method

void ParticleEng::step()
{
    unloadEmitterStack();
    std::vector<int> removeIndices;
    int offset = 0;
    for (std::vector<ParticleEmitter*>::iterator i = activeEmitters.begin(); i < activeEmitters.end(); i += 1) {
        if((*i)->step()) {
            removeIndices.push_back(i - activeEmitters.begin());
            /** create deleter **/
        }
    } // end for (i)
    for (std::vector<int>::iterator i = removeIndices.begin(); i < removeIndices.end(); i += 1) {
        activeEmitters.erase(activeEmitters.begin() + (*i) - offset);
        offset += 1;
    } // end for (i)

    unloadParticleStack();
    removeIndices.clear();
    offset = 0;
    for (std::vector<Particle*>::iterator i = activeParticles.begin(); i < activeParticles.end(); i += 1) {
        if((*i)->step()) {
            removeIndices.push_back(i - activeParticles.begin());
            /** create deleter **/
        }
    } // end for (i)
    for (std::vector<int>::iterator i = removeIndices.begin(); i < removeIndices.end(); i += 1) {
        activeParticles.erase(activeParticles.begin() + (*i) - offset);
        offset += 1;
    } // end for (i)
} // end step method

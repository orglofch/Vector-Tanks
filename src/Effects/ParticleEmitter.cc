#include "ParticleEmitter.h"
#include <cstdlib>

ParticleEmitter::ParticleEmitter(PixMap *pixMap, int duration)
{
    this->pixMap = pixMap;

    maxParticles = 0;
    maxParticleDuration = 0;
    maxParticleVelo = 4;

    startAngle = 0;
    endAngle = 360;

    drag = 0.99f;
    gravity = 0.0f;

    age = 0;
    this->duration = duration;
} // end constructor

void ParticleEmitter::stepParticles()
{
    std::vector<int> removeIndices;
    int offset = 0;
    for (std::vector<Particle*>::iterator i = particles.begin(); i < particles.end(); i += 1) {
        if((*i)->step()) {
            removeIndices.push_back(i - particles.begin());
            /** delete *i; doesn't work **/
        }
    } // end for (i)
    for (std::vector<int>::iterator i = removeIndices.begin(); i < removeIndices.end(); i += 1) {
        particles.erase(particles.begin() + (*i) - offset);
        offset += 1;
    } // end for (i)
} // end stepParticles method

bool ParticleEmitter::dieStep()
{
    if (particles.size() == 0)
        return true;
    stepParticles();
    return false;
} // end dieStep method

void ParticleEmitter::aliveStep() // Continue stepping until all particles have disappeared
{
    stepParticles();
    for (unsigned int i = particles.size(); i < maxParticles; i += 1)
    {
        float newVelo = (float)rand() / (float)RAND_MAX * maxParticleVelo; /** Make a push force value for emitter **/
        float angle = (float)rand() / (float)RAND_MAX * (endAngle - startAngle) + startAngle; /** Angle doesn't work properly yet **/
        Particle *newParticle = new LineParticle(pixMap, pos, createVect(angle, newVelo), rand() % maxParticleDuration + 10, true, false); /** Temporary life span **/
        newParticle->setColor({0.3f, 0.8f, 0.2f, 0.6f}); /** Temporary Color **/
        particles.push_back(newParticle);
    } // end for (i)
} // end aliveStep method

void ParticleEmitter::setPos(Vect pos)
{
    this->pos = pos;
} // end setPos method

void ParticleEmitter::setVelo(Vect velo)
{
    this->velo = velo;
} // end setVelo method

void ParticleEmitter::setAngle(int startAngle, int endAngle)
{
    this->startAngle = startAngle;
    this->endAngle = endAngle;
} // end setAngle method

void ParticleEmitter::setMaxParticles(int maxParticles)
{
    this->maxParticles = maxParticles;
} // end setMaxParticles method

void ParticleEmitter::setMaxParticleDuration(int maxParticleDuration)
{
    this->maxParticleDuration = maxParticleDuration;
} // end setParticleDuration method

void ParticleEmitter::setMaxParticleVelo(float velo)
{
    this->maxParticleVelo = velo;
} // end setMaxParticleVelo method

void ParticleEmitter::setGravity(float gravity)
{
    this->gravity = gravity;
} // end setGravity method

void ParticleEmitter::draw()
{
    for (std::vector<Particle*>::iterator i = particles.begin(); i < particles.end(); i += 1)
        (*i)->draw();
} // end draw method

bool ParticleEmitter::step()
{
    age += 1;
    if (age > duration) {
        if (dieStep())
            return true;
    } else {
        velo *= drag;

        velo.y += gravity;

        pos.x += velo.x;
        pos.y -= velo.y;

        aliveStep();
    }
    return false;
} // end step method

FireEmitter::FireEmitter(PixMap *pixMap, int duration) : ParticleEmitter(pixMap, duration)
{
} // end constructor

void FireEmitter::aliveStep()
{
    stepParticles();
    for (unsigned int i = particles.size(); i < maxParticles; i += 1)
    {
        float newVelo = (float)rand() / (float)RAND_MAX * maxParticleVelo; /** Make a push force value for emitter **/
        float angle = (float)rand() / (float)RAND_MAX * (endAngle - startAngle) + startAngle; /** Angle doesn't work properly yet **/
        Particle *newParticle;
        if (rand() % 3 == 0) {
            newParticle = new LineParticle(pixMap, {pos.x + (float)rand() / (float)RAND_MAX, pos.y + (float)rand() / (float)RAND_MAX}, {newVelo * cos(degreetoRad(angle)),
                                    -newVelo * sin(degreetoRad(angle))}, rand() % maxParticleDuration + 10, true, false); /** Temporary life span **/
        } else {
            newParticle = new CircleParticle(pixMap, {pos.x + (float)rand() / (float)RAND_MAX, pos.y + (float)rand() / (float)RAND_MAX}, {newVelo * cos(degreetoRad(angle)),
                                    -newVelo * sin(degreetoRad(angle))}, rand() % maxParticleDuration + 10, true, false); /** Temporary life span **/
        }
        newParticle->setGravity(0.01f);
        newParticle->setColor({(float)(0.3f + (float)rand() / (float)RAND_MAX * 0.5),(float)(0.3f + (float)rand() / (float)RAND_MAX * 0.5), 0.2f, 1.0f}); /** Temporary Color **/
        particles.push_back(newParticle);
        angle = (float)rand() / (float)RAND_MAX * 40 + 70;
        newVelo = (float)rand() / (float)RAND_MAX / 4;
        newParticle = new SmokeParticle(pixMap, pos, {cos(degreetoRad(angle)) * newVelo, sin(degreetoRad(angle)) * newVelo}, 200, true, false);
        newParticle->setColor({0.2f, 0.2f, 0.2f, 0.08f}); /** Doesn't dissappear properly **/
        particles.push_back(newParticle);
    } // end for (i)
} // end aliveStep method

ElectricEmitter::ElectricEmitter(PixMap *pixMap, int duration) : ParticleEmitter(pixMap, duration)
{
} // end constructor

void ElectricEmitter::aliveStep()
{
    stepParticles();
    for (unsigned int i = particles.size(); i < maxParticles; i += 1)
    {
        float newVelo = (float)rand() / (float)RAND_MAX * maxParticleVelo;
        float angle = (float)rand()  / (float)RAND_MAX * (endAngle - startAngle) + startAngle; /** Angle doesn't work properly yet **/
        Particle *newParticle = new ElectricParticle(pixMap, pos, {cos(degreetoRad(angle)) * newVelo, sin(degreetoRad(angle)) * newVelo}, 60, false);
        newParticle->setColor({0.6f, 0.2f, 0.7f, 1.0f});
        particles.push_back(newParticle);
    } // end for (i)
} // end aliveStep method

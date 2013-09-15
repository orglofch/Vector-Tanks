/**
*
* @author: Owen Glofcheski
* @date: February 27, 2012
*
*/

#ifndef STARFIELD_H_INCLUDED
#define STARFIELD_H_INCLUDED

#include "Globals.h"
#include "Effects/Point.h"
#include "Vect.h"
#include "Math.h"

class StarField {
private:
    int numStars;
    Point *stars;

    float rotVelo;
    Vect pivot;
public:
    StarField(int numStars);

    void draw();
    void step();
};

#endif // STARFIELD_H_INCLUDED

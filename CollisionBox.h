/**
*
* @author: Owen Glofcheski
* @date: March 10, 2012
*
*/

#ifndef COLLISIONBOX_H_INCLUDED
#define COLLISIONBOX_H_INCLUDED

#include "Vect.h"

struct CollisionBox
{
private:
    float getMaxDistance();
public:
    CollisionBox();

    Vect topLeft;
    Vect topRight;
    Vect bottomLeft;
    Vect bottomRight;
    Vect center;

    bool doesCollide(Vect pos);
    bool doesRadialCollide(Vect pos, float radius);
};

#endif // COLLISIONBOX_H_INCLUDED

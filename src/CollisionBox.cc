#include "CollisionBox.h"

CollisionBox::CollisionBox()
{
    topLeft = topRight = bottomLeft = bottomRight = {0, 0};
} // end default contructor

float CollisionBox::getMaxDistance()
{
    return distance(topLeft, topRight) + distance(topLeft, bottomLeft) + distance(topLeft, bottomRight);
} // end getMaxDistance method

bool CollisionBox::doesCollide(Vect pos)
{
    float totDistance = distance(pos, topLeft) + distance(pos, topRight) + distance(pos, bottomLeft) + distance(pos, bottomRight);
    if (totDistance > getMaxDistance())
        return false;
    return true;
} // end doesCollide method

bool CollisionBox::doesRadialCollide(Vect pos, float radius)
{
    if (distance(pos, center) < radius) /** Make better, this is very niave as it is right now **/
        return true;
    return false;
} // end doeRadialCollide

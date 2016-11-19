/**
*
* @author: Owen Glofcheski
* @date: February 14, 2012
*
*/

#ifndef POINT_H_
#define POINT_H_

#include "../Color.h"
#include "../Vect.h"

class Point
{
protected:
	Vect cp, lp, a;
	Color color;
	float lt, mass, size;
public:
    Point();
    Point(Vect position, float mass);
	virtual ~Point();

	Vect getVelocity();
	Vect getPosition();
    float getMass();
    Color getColor();

    void setPosition(Vect pos);
    void setColor(Color color);
    void setSize(float size);

	virtual void draw();
    virtual void step(float dt);
	virtual void addForce(Vect force);
};

#endif // POINT_H_INCLUDED

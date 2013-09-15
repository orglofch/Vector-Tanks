/**
*
* @author: Owen Glofcheski
* @date: February 14, 2012
*
*/

#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_

#include "point.h"
#include "../Color.h"
#include "../Vect.h"

class Constraint
{
private:
    Color color;

	Point *p1, *p2; // The points the contraint connects
	float k, rest;

public:
	Constraint(Point *p1, Point *p2, float k, float rest, Color color);

	void draw();
	void step(float dt);
};

#endif // CONTSRAINT_H_INCLUDED

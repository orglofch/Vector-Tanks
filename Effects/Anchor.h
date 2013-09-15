/**
*
* @author: Owen Glofcheski
* @date: February 14, 2012
*
*/

#ifndef ANCHOR_H_
#define ANCHOR_H_

#include "point.h"
#include "../Vect.h"

class Anchor : public Point
{
public:
	Anchor() {};
	Anchor(Vect position);
    ~Anchor();

	void draw();
    void step(float dt);
	void addForce(Vect force);
};

#endif // ANCHOR_H_INCLUDED

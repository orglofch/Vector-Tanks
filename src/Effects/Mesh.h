/**
*
* @author: Owen Glofcheski
* @date: February 17, 2012
*
*/

#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <vector>

#include "point.h"
#include "constraint.h"
#include "anchor.h"
#include "../Globals.h"
#include "../Math.h"
#include "../Vect.h"

class Mesh
{
private:
    int gridSize;
    int gridWidth, gridHeight;

    std::vector <Point*>  points;
	std::vector <Constraint*> constraints;

	bool isOn;
public:
    Mesh();
    ~Mesh();

    void turnOn();
    void turnOff();

    void draw();
    void step();
    void applyForce(Vect mPos, int force, int startAngle, int endAngle);
};

#endif // MESH_H_INCLUDED

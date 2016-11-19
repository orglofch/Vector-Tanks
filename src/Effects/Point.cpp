#include "Point.h"
#include <gl/gl.h>
#include <cmath>

Point::Point() {
    color.red = color.green = color.blue = color.alpha = 0.0f;
    size = 0.0f;
} // end default constructor

Point::Point(Vect position, float mass)
{
    size = 0.0f;
	cp = position;
	lp = cp;
	lt = 0;
	a = Vect(0,0);
	this->mass = mass;
} // end constructor

Point::~Point(){
} // end destructor

Vect Point::getVelocity()
{
	return Vect((cp.x - lp.x) / lt, (cp.y - lp.y) / lt);
} // end getVelocity method

Vect Point::getPosition()
{
	return cp;
} // end getPosition method

float Point::getMass()
{
	return mass;
} // end getMass method

Color Point::getColor()
{
    return color;
} // end getColor method

void Point::setPosition(Vect pos)
{
	cp = pos;
	lp = pos;
} // end setPosition method

void Point::setColor(Color color)
{
    this->color.red = color.red;
    this->color.green = color.green;
    this->color.blue = color.blue;
    this->color.alpha = color.alpha;
} // end set COor method

void Point::setSize(float size)
{
    this->size = size;
} // end setSize method

void Point::draw()
{
    glColor4f(color.red, color.green, color.blue, color.alpha);
    glBegin(GL_QUADS);
        glVertex2f(cp.x - size / 2, cp.y - size / 2);
        glVertex2f(cp.x - size / 2, cp.y + size / 2);
        glVertex2f(cp.x + size / 2, cp.y + size / 2);
        glVertex2f(cp.x + size / 2, cp.y - size / 2);
    glEnd();
} // end draw method

void Point::step(float dt)
{
	if(lt == 0)
        lt = dt;

	const float drag = 0.5;

	Vect vel = getVelocity();
	addForce(Vect(-vel.x * drag, -vel.y * drag));


	Vect n;
	n.x = cp.x + (cp.x - lp.x) * (dt / lt) + a.x * dt * dt;
	n.y = cp.y + (cp.y - lp.y) * (dt / lt) + a.y * dt * dt;

	lp = cp;
	cp = n;

	lt = dt;

	a = Vect(0,0);
} // end step method

void Point::addForce(Vect force)
{
	a.x += force.x / mass;
	a.y += force.y / mass;
} // end addForce method


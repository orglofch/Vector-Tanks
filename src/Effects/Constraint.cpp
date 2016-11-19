#include "Constraint.h"
#include <gl/gl.h>
#include <cmath>

Constraint::Constraint(Point *p1, Point *p2, float k, float rest, Color color)
{
	this->p1 = p1;
	this->p2 = p2;
	this->k = k;
	this->rest = rest;

	this->color.red = color.red;
	this->color.green = color.green;
	this->color.blue = color.blue;
	this->color.alpha = color.alpha;
} // end constructor

void Constraint::draw()
{
    float length;
	Vect v1 = p1->getPosition();
	Vect v2 = p2->getPosition();
	Vect res(v1.x - v2.x, v1.y - v2.y);
	length = sqrt(res.x * res.x + res.y * res.y);
    glColor4f(color.red / pow(length, 4) * 700, color.green / pow(length, 4) * 700, color.blue / pow(length, 4) * 700, 1.0f); /** Not finalized **/
    glBegin(GL_LINES);
        glVertex2f(p1->getPosition().x, p1->getPosition().y);
        glVertex2f(p2->getPosition().x, p2->getPosition().y);
    glEnd();
} // end draw method

void Constraint::step(float dt)
{
	float force, length;
	Vect v1 = p1->getPosition();
	Vect v2 = p2->getPosition();
	Vect res(v1.x - v2.x, v1.y - v2.y);
	length = sqrt(res.x * res.x + res.y * res.y);
	if(length < 1)
        length = 1;
	force = k * (length - rest);

	Vect dirForce((res.x / length) * force,(res.y / length) * force);
	p2->addForce(dirForce);
	p1->addForce(Vect(-dirForce.x, -dirForce.y));
} // end step method

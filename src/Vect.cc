#include "Vect.h"
#include <gl/gl.h>

Vect::Vect()
{
    x = y = 0;
} // end default constructor

Vect::Vect(double x, double y)
{
    this->x = x;
    this->y = y;
} // end constructor

Vect::Vect(double x, int y)
{
    this->x = x;
    this->y = (double)y;
} // end constructor

Vect::Vect(int x, double y)
{
    this->x = (double)x;
    this->y = y;
} // end constructor

Vect::Vect(int x, int y)
{
    this->x = (double)x;
    this->y = (double)y;
} // end constructor

float Vect::getTotalVelo() const
{
    return sqrt(pow(x, 2) + pow(y, 2));
} // end getTotalVeloMethod

float Vect::getAngle() const
{
    return radtoDegree(atan2(-y, x));
} // end getAngle method

void Vect::operator+= (float num)
{
    float totVelo = getTotalVelo() * num;
    float angle = degreetoRad(getAngle());
    x = cos(angle) * totVelo;
    y = -sin(angle) * totVelo;
} // end operator*= method

void Vect::operator-= (float num)
{
    float totVelo = getTotalVelo() * num;
    float angle = degreetoRad(getAngle());
    x = cos(angle) * totVelo;
    y = -sin(angle) * totVelo;
} // end operator*= method

void Vect::operator*= (float num)
{
    float totVelo = getTotalVelo() * num;
    float angle = degreetoRad(getAngle());
    x = cos(angle) * totVelo;
    y = -sin(angle) * totVelo;
} // end operator*= method

void Vect::operator/= (float num)
{
    float totVelo = getTotalVelo() * num;
    float angle = degreetoRad(getAngle());
    x = cos(angle) * totVelo;
    y = -sin(angle) * totVelo;
} // end operator*= method

void Vect::operator+= (const Vect &vector)
{
    x += vector.x;
    y += vector.y;
} // end operator+= method

void Vect::operator-= (const Vect &vector)
{
    x -= vector.x;
    y -= vector.y;
} // end operator -= method

void Vect::operator*= (const Vect &vector)
{
    x *= vector.x;
    y *= vector.y;
} // end operator *= method

void Vect::operator/= (const Vect &vector)
{
    x /= vector.x;
    y /= vector.y;
} // end operator /= method

void glVertexVect(Vect pos)
{
    glVertex2f(pos.x, pos.y);
} // end glVertexVect function

Vect operator+ (const Vect &vector)
{
    return Vect(fabs(vector.x), fabs(vector.y));
} // end operator+ method

Vect operator+ (const Vect &vector, int num)
{
    float totVelo = vector.getTotalVelo() + num;
    float angle = degreetoRad(vector.getAngle());
    return {cos(angle) * totVelo, -sin(angle) * totVelo};
} // end operator+ function

Vect operator+ (int num, const Vect &vector)
{
    return vector + num;
} // end operator+ function

Vect operator+ (const Vect &v1, const Vect &v2)
{
    return Vect(v1.x + v2.x, v1.y + v2.y);
} // end operator+ function

Vect operator- (const Vect &vector)
{
    return Vect(vector * -1);
} // end operator- method

Vect operator- (const Vect &vector, int num)
{
    float totVelo = vector.getTotalVelo() - num;
    float angle = degreetoRad(vector.getAngle());
    return {cos(angle) * totVelo, -sin(angle) * totVelo};
} // end operator- function

Vect operator- (int num, const Vect &vector)
{
    return vector - num;
} // end operator- function

Vect operator- (const Vect &v1, const Vect &v2)
{
    return Vect(v1.x - v2.x, v1.y - v2.y);
} // end operator- function

Vect operator/ (const Vect &vector, int num)
{
    float totVelo = vector.getTotalVelo() / num;
    float angle = degreetoRad(vector.getAngle());
    return {cos(angle) * totVelo, -sin(angle) * totVelo};
} // end operator/ function

Vect operator/ (int num, const Vect &vector)
{
    return vector / num;
} // end operator/ function

Vect operator/ (const Vect &v1, const Vect &v2)
{
    return Vect(v1.x / v2.x, v1.y / v2.y);
} // end operator/ function

Vect operator* (const Vect &vector, int num)
{
    float totVelo = vector.getTotalVelo() * num;
    float angle = degreetoRad(vector.getAngle());
    return {cos(angle) * totVelo, -sin(angle) * totVelo};
} // end operator* function

Vect operator* (int num, const Vect &vector)
{
    return vector * num;
} // end operator* function

Vect operator* (const Vect &v1, const Vect &v2)
{
    return Vect(v1.x * v2.x, v1.y * v2.y);
} // end operator* function

bool operator== (const Vect &v1, const Vect &v2)
{
    return (v1.x == v2.x && v1.y == v2.y);
} // end operator== function

bool operator!= (const Vect &v1, const Vect &v2)
{
    return(v1.x != v2.x || v1.y != v2.y);
} // end operator!= function

float distance(const Vect &pos1, const Vect &pos2)
{
    return (sqrt(fabs(pow(fabs(pos1.x - pos2.x), 2) + pow(fabs(pos1.y - pos2.y), 2))));
} // end distance function

float angleBetween(const Vect &pos1, const Vect &pos2)
{
    return radtoDegree(atan2((pos1.y - pos2.y), (pos2.x - pos1.x)));
} // end angle function

Vect createVect(float angle, float totVelo) /** Doesn't work...why not...? **/
{
    return Vect(cos(degreetoRad(angle)) * totVelo, -sin(degreetoRad(angle)) * totVelo);
} // end createVect method

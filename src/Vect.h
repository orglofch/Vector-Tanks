/**
*
* @author: Owen Glofcheski
* @date: March 3, 2012
*
*/

#ifndef VECT_H_INCLUDED
#define VECT_H_INCLUDED

#include "Math.h"

struct Vect
{
    double x, y;

    Vect();
    Vect(double x, double y);
    Vect(double x, int y);
    Vect(int x, double y);
    Vect(int x, int y);

    void operator+= (float num);
    void operator-= (float num);
    void operator*= (float num);
    void operator/= (float num);

    void operator+= (const Vect &vector);
    void operator-= (const Vect &vector);
    void operator*= (const Vect &vector);
    void operator/= (const Vect &vector);

    float getTotalVelo() const; // only to be used when Vect is representing velocity
    float getAngle() const; // Only to be used when Vect is representing velocity
};

extern void glVertexVect(Vect pos);

Vect operator+ (const Vect &vector);
Vect operator+ (const Vect &vector, int num);
Vect operator+ (int num, const Vect &vector);
Vect operator+ (const Vect &v1, const Vect &v2);

Vect operator- (const Vect &vector);
Vect operator- (const Vect &vector, int num);
Vect operator- (int num, const Vect vector);
Vect operator- (const Vect &v1, const Vect &v2);

Vect operator/ (const Vect &vector, int num);
Vect operator/ (int num, const Vect &vector);
Vect operator/ (const Vect &v1, const Vect &v2);

Vect operator* (const Vect &vector, int num);
Vect operator* (int num, const Vect &vector);
Vect operator* (const Vect &v1, const Vect &v2);

bool operator== (const Vect &v1, const Vect &v2);
bool operator!= (const Vect &v1, const Vect &v2);

float distance(const Vect &pos1, const Vect &pos2);
float angleBetween(const Vect &pos1, const Vect &pos2);

Vect createVect(float angle, float totVelo);

#endif // VECT_H_INCLUDED

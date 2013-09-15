/**
*
* @author: Owen Glofcheski
* @date: March 3, 2012
*
*/

#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include <gl/gl.h>

struct Color {
    float red;
    float green;
    float blue;
    float alpha;

    Color();
    Color(float red, float green, float blue, float alpha);
};

extern void glColor(Color color);

#endif // COLOR_H_INCLUDED

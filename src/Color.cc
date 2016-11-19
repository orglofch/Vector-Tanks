#include "Color.h"

Color::Color() {
    red = blue = green = alpha = 0.0f;
} // end default constructor

Color::Color(float red, float green, float blue, float alpha) {
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;
} // end constructor

void glColor(Color color)
{
    glColor4f(color.red, color.green, color.blue, color.alpha);
} // end glColor method


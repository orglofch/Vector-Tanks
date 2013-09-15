#include "StarField.h"
#include <cstdlib>

StarField::StarField(int numStars)
{
    pivot.x  = mapWidth / 2;
    pivot.y = mapHeight * 2;
    rotVelo = 0.4f;

    this->numStars = numStars;
    stars = new Point [numStars];
    for (int i = 0; i < numStars; i += 1)
    {
        Vect pos;
        pos.x = rand() % mapWidth;
        pos.y = rand() % mapHeight;
        stars [i].setPosition(pos); /** Make it actually use vectors **/
        /** Make it so stars near the bottom of the screen are nore transparent **/
        if (i % 15 == 0) {
            stars [i].setColor({1.0f, 1.0f, 1.0f, 0.8f});
            stars [i].setSize(2.0f);
        } else if (i % 15 < 5) {
            stars [i].setColor({1.0f, 1.0f, 1.0f, 0.6f});
            stars [i].setSize(1.0f);
        } else {
            stars [i].setColor({1.0f, 1.0f, 1.0f, 0.2f});
            stars [i].setSize(1.0f);
        }
    } // end for (i)
} // end constructor

void StarField::draw()
{
    for (int i = 0; i < numStars; i += 1)
        stars [i].draw();
} // end draw method

void StarField::step()
{
    for (int i = 0; i < numStars; i += 1)
    {
        Vect pos = stars[i].getPosition();
        float angle = radtoDegree(atan2((pivot.y - pos.y), (pos.x - pivot.x)));
        Color color = stars[i].getColor();
        if (color.alpha == 0.8f) {
            pos.x += cos(degreetoRad(angle + 90)) * rotVelo;
            pos.y -= sin(degreetoRad(angle + 90)) * rotVelo;
        } else if (color.alpha == 0.6f) {
            pos.x += cos(degreetoRad(angle + 90)) * rotVelo / 2;
            pos.y -= sin(degreetoRad(angle + 90)) * rotVelo / 2;
        } else if (color.alpha == 0.2f) {
            pos.x += cos(degreetoRad(angle + 90)) * rotVelo / 3;
            pos.y -= sin(degreetoRad(angle + 90)) * rotVelo / 3;
        }
        if (pos.x < 0) {
            pos.x = mapWidth - 1;
        } else if (pos.x > mapWidth - 1) {
            pos.y = 0;
        }
        if (pos.y < 0) {
            pos.y = mapHeight - 1;
        } else if (pos.y > mapHeight - 1) {
            pos.y = 0;
        }
        stars[i].setPosition(pos);
    } // end for (i)
} // end step method

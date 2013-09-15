/**
*
* @author: Owen Glofcheski
* @date: February 14, 2012
*
*/

#ifndef FRAMERATE_H
#define FRAMERATE_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class Framerate
{
private:
    float targetfps;
    float fps;

    LARGE_INTEGER tickspersecond;
    LARGE_INTEGER currentticks;
    LARGE_INTEGER framedelay;

    float speedfactor;

public:
    void init(float tfps);

    float getFps();
    float getSpeedFactor();

    void slower();
    void faster();

    void setTargetFps(float t);

    void updateSpeedFactor();
};
#endif // FRAMERATE_H_INCLUDED

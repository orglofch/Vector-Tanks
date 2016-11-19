#include "framerate.h"

void Framerate::init(float tfps)
{
	targetfps = tfps;
	QueryPerformanceCounter(&framedelay);
	QueryPerformanceFrequency(&tickspersecond);
} // end init method

void Framerate::updateSpeedFactor()
{
	QueryPerformanceCounter(&currentticks);
	speedfactor = (float)(currentticks.QuadPart-framedelay.QuadPart) /
                    ((float)tickspersecond.QuadPart / targetfps);
	fps = targetfps / speedfactor;
	if (speedfactor <= 0)
        speedfactor = 1;
	framedelay = currentticks;
} // end updateSpeedFactor method

void Framerate::setTargetFps(float t)
{
	targetfps = t;
} // end setTargetFps method

float Framerate::getFps()
{
	return fps;
} // end getFps method

float Framerate::getSpeedFactor()
{
	return speedfactor;
} // end getSpeedFactor method

void Framerate::slower()
{
    targetfps -= 0.01;
} // end slower method

void Framerate::faster()
{
    targetfps += 0.01;
} // end faster method

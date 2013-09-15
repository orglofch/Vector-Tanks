/**
*
* @author: Owen Glofcheski
* @date: February 5, 2012
*
*/

#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <SDL/SDL.h>
#include "glfont.h"
#include <gl/gl.h>
#include <gl/glext.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "Framerate.h"
#include "StarField.h"
#include "Player.h"
#include "PixMap.h"
#include "Globals.h"
#include "Effects.h"
#include "Vect.h"
#include "PixNode.h"
#include "Utility.h"

class System
{
private:
    /** Create FPS limiter **/

    SDL_Surface *drawContext;
    SDL_Event event;

    // Level Objects
	Framerate fRate;
	PixMap pixMap;
	StarField starField;

    int numPlayers;
    int currentPlayerIndex;
	Player *currentPlayer;
	std::vector<Player*> players;

	float timeToUpdate;
	float timestep;
	float dt;

    std::vector<int> heldButtons;
    std::vector<int> heldKeys;

    GLuint windowTexture;
    GLuint backgroundTexture;

    bool isDone;

    void draw();
    void flip();
    void step();

    bool SDLInit();
    bool GLInit();
    bool PlayerInit();

    void setShaders();
    void setWindowTexture();
    void nextPlayer();
public:
    System();
    ~System();

    bool init();

    void run();
};

#endif // SYSTEM_H_INCLUDED

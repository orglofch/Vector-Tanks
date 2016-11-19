#include "System.h"

System::System() : pixMap(), starField((mapWidth + mapHeight) / 2)
{
    pixMap.genPixMap(SANDSEA); /** Temporary **/

    numPlayers = 2;

    isDone = false;
    fRate.init(1.0f);

    timeToUpdate = 0;
	timestep = 0.01;
	dt = timestep;
	backgroundTexture = NULL;
} // end default constructor

System::~System()
{
    for (std::vector<Player*>::iterator i = players.begin(); i < players.end(); i += 1) {
        delete (*i);
    } // end for (i)
    SDL_Quit();
} // end destructor

void System::setShaders()
{
    /** possibly implement **/
} // end setShaders method

void System::setWindowTexture()
{
    glGenTextures(1, &windowTexture);
    glBindTexture(GL_TEXTURE_2D, windowTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    GLubyte screenData[mapWidth][mapHeight][4];
    glReadPixels(0, 50, mapWidth, mapHeight, GL_RGBA, GL_UNSIGNED_BYTE, screenData);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mapWidth, mapHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, screenData);
} // end loadWindowTexture method

void System::nextPlayer()
{
    if (players.size() > 0) {
        if (currentPlayerIndex >= players.size() - 1) {
            currentPlayerIndex = 0;
        } else {
            currentPlayerIndex += 1;
        }
        currentPlayer = players[currentPlayerIndex];
        currentPlayer->resetTurnTimer(); /** Something else is also suppose to do this **/
    } else {
        currentPlayer = NULL;
    }
} // end nextPlayer method

void System::run()
{
    while(!isDone)
    {
        step(); // Run on iteration of the game

        draw(); // Draw game with new positions to buffer

        flip(); // Flip the buffer to the screen
    } // end while
} // end run method

bool System::SDLInit()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Failed to initialize SDL" << std::endl;
        return false;
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    int flags = SDL_OPENGL | SDL_NOFRAME; // other flags: SDL_NOFRAME | SDL_FULLSCREEN;
    drawContext = SDL_SetVideoMode(WINWIDTH, WINHEIGHT, 0, flags);
    if (!drawContext) {
        std::cout << "SDL_SetVideoMode failed" << std::endl;
        return false;
    }

    return true;
} // end init method

bool System::GLInit()
{
    glDisable(GL_TEXTURE_2D);
    if(!loadTexture("Media/base2.bmp", backgroundTexture)) { /** Move somewhere else **/
        return false;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (0, WINWIDTH, WINHEIGHT, 0, 0, 1);
    glMatrixMode (GL_MODELVIEW);

    glEnable (GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);

    glLineWidth(1); /** Temporary, mess around with this **/

	setShaders();
	return true;
} // end GLInit method

bool System::PlayerInit()
{
    // set Players
    Player *newPlayer;
    for (int i = 0; i < numPlayers; i += 1) {
        newPlayer = new Tank(&pixMap, i + 1);
        newPlayer->setPosition({mapWidth / (numPlayers + 1) * (i + 1), 20}); /** Make more randomly positioned **/
        newPlayer->setColor({(float)rand() / (float)RAND_MAX * 0.7f + 0.1f,(float)rand() / (float)RAND_MAX * 0.7f + 0.1f, (float)rand() / (float)RAND_MAX * 0.7f + 0.1f, 1.0f});
        players.push_back(newPlayer);
    } // end for (i)
    pixMap.addActivePlayers(&players);
    if (players.size() > 0) {
        currentPlayerIndex = 0; /** make random **/
        currentPlayer = players[currentPlayerIndex];
    } else {
        currentPlayer = NULL;
    }
    return true; /** Remove if no chance of false **/
} // end PlayerInit

bool System::init()
{
    if (!SDLInit()) {
        return false;
    }
    if (!GLInit()) {
        return false;
    }
    if (!PlayerInit()) {
        return false;
    }
    return true;
} // end init method

void System::draw()
{
    glClear (GL_COLOR_BUFFER_BIT); //clear pixel buffer

    glEnable(GL_TEXTURE_2D); /** make draw background method**/
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0);
        glVertex2f(0.0f, 0.0f);

        glTexCoord2i(1, 0);
        glVertex2f(mapWidth, 0.0f);

        glTexCoord2i(1, 1);
        glVertex2f(mapWidth, mapHeight - 1);

        glTexCoord2i(0, 1);
        glVertex2f(0.0f, mapHeight - 1);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //starField.draw();
    mesh.draw();
    pixMap.draw();
    pEng.draw();
    for (std::vector<Player*>::iterator i = players.begin(); i < players.end(); i += 1) {
        (*i)->draw();
    } // end for (i)
    if (currentPlayer != NULL) {
        currentPlayer->drawIcon();
        currentPlayer->drawUI();
    }
} // end draw method

void System::flip()
{
    SDL_GL_SwapBuffers();
} // end flip method

void System::step()
{
    int x, y;

    fRate.updateSpeedFactor();
    float tmpDt = fRate.getSpeedFactor();
    timeToUpdate += tmpDt;

    while(timeToUpdate > timestep) /** possible make if, or remake fps **/
    {
        SDL_GetMouseState(&x, &y);
        Vect mPos(x, y);
        if (currentPlayer != NULL) {
            Vect playerPos = currentPlayer->getPosition();
            currentPlayer->setAngle(radtoDegree(atan2(mPos.y - playerPos.y, mPos.x - playerPos.x)));
        }

        timeToUpdate -= 0.01;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT: {
                    isDone = true;
                    break;
                } case SDL_KEYUP: {
                    for (std::vector<int>::iterator i = heldKeys.begin(); i < heldKeys.end(); i += 1)
                    {
                        if (*i == event.key.keysym.sym)
                            heldKeys.erase(i);
                    } // end for (i)
                    break;
                } case SDL_KEYDOWN: {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        isDone = true;
                    } else if (event.key.keysym.sym == SDLK_SPACE) {
                        if (currentPlayer != NULL) {
                            currentPlayer->fireProjectile();
                        }
                    } else if (event.key.keysym.sym == SDLK_x) {
                        if (currentPlayer != NULL) {
                            currentPlayer->kill(); /** Temporary **/
                        }
                    } else if (event.key.keysym.sym == SDLK_v) { /** Temporary **/
                        pixMap.activateProjectiles();
                    } else if (event.key.keysym.sym == SDLK_e) {
                        if (currentPlayer != NULL) {
                            currentPlayer->nextProjectile();
                        }
                    } else if (event.key.keysym.sym == SDLK_p) {
                    } else {
                        heldKeys.push_back(event.key.keysym.sym);
                    }
                    break;
                } case SDL_MOUSEBUTTONUP: {
                    for (std::vector<int>::iterator i = heldButtons.begin(); i < heldButtons.end(); i += 1)
                    {
                        if (*i == event.button.button)
                            heldButtons.erase(i);
                    } // end for (i)
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        if (currentPlayer != NULL) {
                            currentPlayer->release(mPos);
                        }
                    }
                    break;
                } case SDL_MOUSEBUTTONDOWN: {
                    if (event.button.button == SDL_BUTTON_WHEELDOWN) {
                        currentPlayer->previousProjectile();
                    } else if (event.button.button == SDL_BUTTON_WHEELUP) {
                        currentPlayer->nextProjectile();
                    } else if (event.button.button == SDL_BUTTON_LEFT) {
                        if (currentPlayer != NULL) {
                            currentPlayer->click(mPos);
                        }
                    } else {
                        heldButtons.push_back(event.button.button);
                    }
                    break;
                }
            } // end switch
        } // end while
        for (std::vector<int>::iterator i = heldButtons.begin(); i < heldButtons.end(); i += 1)
        {
            if (*i == SDL_BUTTON_LEFT) {
            } else if (*i == SDL_BUTTON_RIGHT) {
                pixMap.pushLiquify(mPos, 25);
            } else if (*i == SDL_BUTTON_MIDDLE) {
                glScalef(1.01f, 1.01f, 1.0f); /** Temporary **/
            }
        } // end for (i)
        for (std::vector<int>::iterator i = heldKeys.begin(); i < heldKeys.end(); i += 1)
        {
            if (*i == SDLK_DOWN) {
                if (currentPlayer != NULL) {
                    currentPlayer->setAngle(currentPlayer->getAngle() + 0.5);
                }
            } else if (*i == SDLK_UP) {
                if (currentPlayer != NULL) {
                    currentPlayer->setAngle(currentPlayer->getAngle() - 0.5);
                }
            } else if (*i == SDLK_LEFT) {
                mesh.turnOff();
            } else if (*i == SDLK_RIGHT) {
                mesh.turnOn();
            } else if (*i == SDLK_SPACE) {
            } else if (*i == SDLK_s) {
            } else if (*i == SDLK_w) {
                if (currentPlayer != NULL) {
                    currentPlayer->move({0.0f, 0.15f});
                }
            } else if (*i == SDLK_a) {
                if (currentPlayer != NULL) {
                    currentPlayer->move({-0.05f, 0.0f});
                }
            } else if (*i == SDLK_d) {
                if (currentPlayer != NULL) {
                    currentPlayer->move({0.05f, 0.0f});
                }
            }
        } // end for (i)
        starField.step();
        mesh.step();
        pixMap.step();
        pEng.step();
        for (std::vector<Player*>::iterator i = players.begin(); i < players.end(); i += 1) {
            if ((*i)->step()) {
                players.erase(i);
                /** Create deleter **/
                int eraseIndex = i - players.begin();
                if (eraseIndex == currentPlayerIndex) {
                    currentPlayerIndex -= 1;
                    nextPlayer();
                }
            }
        } // end for (i)
        if (currentPlayer != NULL) {
            currentPlayer->stepIcon();
            currentPlayer->stepUI(mPos);
            if(currentPlayer->checkTurnTimer()) {
                currentPlayer->resetTurnTimer();
                nextPlayer();
            }
        }
    } // end while
} // end step method

#include "UIHandler.h"

void UIHandler::draw()
{
    for (std::vector <UIEntity *>::iterator i = entities.begin(); i < entities.end(); i += 1) {
        (*i)->draw((*i)->getPos(), 1.0f);
    } // end for (i)
} // end draw method

void UIHandler::step(Vect pos)
{
    for (std::vector <UIEntity *>::iterator i = entities.begin(); i < entities.end(); i += 1) {
        (*i)->step(pos);
    } // end for (i)
} // end step method

void UIHandler::click(Vect pos)
{
    for (std::vector <UIEntity *>::iterator i = entities.begin(); i < entities.end(); i += 1) {
        if ((*i)->checkCollide(pos))
            (*i)->onClick(pos);
    } // end for (i)
} // end click method

void UIHandler::release(Vect pos)
{
    for (std::vector <UIEntity *>::iterator i = entities.begin(); i < entities.end(); i += 1) {
        if ((*i)->checkCollide(pos))
            (*i)->onRelease(pos);
    } // end for (i)
} // end click method

void UIHandler::addEntity(UIEntity *entity)
{
    entities.push_back(entity);
} // end addEntity method

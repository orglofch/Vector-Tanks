/**
*
* @author: Owen Glofcheski
* @date: April 10th, 2012
*
*/

#ifndef UIHANDLER_H_INCLUDED
#define UIHANDLER_H_INCLUDED

#include <vector>

#include "UIEntity.h"

class UIHandler
{
private:
    std::vector <UIEntity *> entities;
public:
    void draw();
    void step(Vect pos);

    void click(Vect pos);
    void release(Vect pos);

    void addEntity(UIEntity *entity);
};

#endif // UIHANDLER_H_INCLUDED

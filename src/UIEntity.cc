#include "UIEntity.h"
#include <iostream>

UIEntity::UIEntity(Vect pos, float width, float height, float alpha, bool isHidden, UIAction hideAction)
{
    this->pos = pos;
    color = Color(1.0f, 1.0f, 1.0f, 1.0f); /** Temporary **/
    endWidth = currentWidth = width;
    endHeight = currentHeight = height;
    endAlpha = currentAlpha = alpha;

    action = NONE;
    this->isHidden = isHidden;
    if (this->isHidden) {
        if (hideAction == HIDE) {
            currentWidth = 0;
            currentHeight = 0;
        } else if (hideAction == SLIDEUPHIDE) {
            currentHeight = 0;
        } else if (hideAction == SLIDEDOWNHIDE) {
            this->pos.y += currentHeight;
            currentHeight = 0;
        } else if (hideAction == SLIDELEFTHIDE) {
            currentWidth = 0;
        } else if (hideAction == SLIDERIGHTHIDE) {
            this->pos.x += currentWidth;
            currentWidth = 0;
        } else if (hideAction == FADEOUT) {
            currentAlpha = 0.0f;
        }
    }
    texture = NULL;
    defaultTexture = NULL;
    depressedTexture = NULL;
    toolTip = NULL;
    color = Color(0.0f, 0.0f, 0.0f, 0.0f);
} // end constructor

UIEntity::~UIEntity()
{
    delete toolTip;
} // end destructor

void UIEntity::reset(float speed)
{
    currentSteps = 0;
    endSteps = speed;
} // end reset method

void UIEntity::hideStep()
{
    if (action == HIDE) {
        currentWidth -= widthInc;
        currentHeight -= heightInc;
    } else {
        currentWidth += widthInc;
        currentHeight += heightInc;
    }
} // end hideStep method

void UIEntity::slideUpStep()
{
    if (action == SLIDEUPHIDE) {
        currentHeight -= heightInc;
    } else {
        currentHeight += heightInc;
    }
} // end slideStep method

void UIEntity::slideDownStep()
{
    if (action == SLIDEDOWNHIDE) {
        pos.y += heightInc;
        currentHeight -= heightInc;
    } else {
        pos.y -= heightInc;
        currentHeight += heightInc;
    }
} // end slideDownStep method

void UIEntity::slideLeftStep()
{
    if (action == SLIDELEFTHIDE) {
        currentWidth -= widthInc;
    } else {
        currentWidth += widthInc;
    }
} // end slideDownStep method

void UIEntity::slideRightStep()
{
    if (action == SLIDERIGHTHIDE) {
        pos.x += widthInc;
        currentWidth -= widthInc;
    } else {
        pos.x -= widthInc;
        currentWidth += widthInc;
    }
} // end slideDownStep method

void UIEntity::fadeStep()
{
    if (action == FADEOUT) {
        currentAlpha -= alphaInc;
    } else {
        currentAlpha += alphaInc;
    }
} // end fadeStep method

void UIEntity::hide(float speed)
{
    action = HIDE;
    widthInc = currentWidth / speed;
    heightInc = currentHeight / speed;
    reset(speed);
} // end hide method

void UIEntity::show(float speed)
{
    isHidden = false;
    action = SHOW;
    widthInc = (endWidth - currentWidth) / speed; /** Possibly Inc should be the same when hide/show**/
    heightInc = (endHeight - currentHeight) / speed; /** Possibly Inc should be the same when hide/show**/
    reset(speed);
} // end show method

void UIEntity::toggle(float speed)
{
    if (isHidden || action == HIDE) {
        action = SHOW;
        isHidden = false;
        widthInc = (endWidth - currentWidth) / speed;
        heightInc = (endHeight - currentHeight) / speed;
    } else {
        action = HIDE;
        widthInc = currentWidth / speed;
        heightInc = currentHeight / speed;
    }
    reset(speed);
} // end toggle method

void UIEntity::slideUpHide(float speed)
{
    action = SLIDEUPHIDE;
    heightInc = currentHeight / speed;
    reset(speed);
} // end slideHideDown method

void UIEntity::slideUpShow(float speed)
{
    isHidden = false;
    action = SLIDEUPSHOW;
    heightInc = (endHeight - currentHeight) / speed;
    reset(speed);
} // end slideShowDown method

void UIEntity::toggleUpSlide(float speed)
{
    if (isHidden || action == SLIDEUPHIDE) {
        action = SLIDEUPSHOW;
        isHidden = false;
        heightInc = (endHeight - currentHeight) / speed;
    } else {
        action = SLIDEUPHIDE;
        heightInc = currentHeight / speed;
    }
    reset(speed);
} // end toggleSlide method

void UIEntity::slideDownHide(float speed)
{
    action = SLIDEDOWNHIDE;
    heightInc = currentHeight / speed;
    reset(speed);
} // end slideHideDown method

void UIEntity::slideDownShow(float speed)
{
    isHidden = false;
    action = SLIDEDOWNSHOW;
    heightInc = (endHeight - currentHeight) / speed;
    reset(speed);
} // end slideShowDown method

void UIEntity::toggleDownSlide(float speed)
{
    int speedDif;
    if (isHidden || action == SLIDEDOWNHIDE) {
        action = SLIDEDOWNSHOW;
        isHidden = false;
        heightInc = (endHeight - currentHeight) / speed;
    } else {
        action = SLIDEDOWNHIDE;
        heightInc = currentHeight / speed;
    }
    reset(speed);
} // end toggleSlide method

void UIEntity::slideLeftHide(float speed)
{
    action = SLIDELEFTHIDE;
    widthInc = currentWidth / speed;
    reset(speed);
} // end slideHideDown method

void UIEntity::slideLeftShow(float speed)
{
    isHidden = false;
    action = SLIDELEFTSHOW;
    widthInc = (endWidth - currentWidth) / speed;
    reset(speed);
} // end slideShowDown method

void UIEntity::toggleLeftSlide(float speed)
{
    if (isHidden || action == SLIDELEFTHIDE) {
        action = SLIDELEFTSHOW;
        isHidden = false;
        widthInc = (endWidth - currentWidth) / speed;
    } else {
        action = SLIDELEFTHIDE;
        widthInc = currentWidth / speed;
    }
    reset(speed);
} // end toggleSlide method

void UIEntity::slideRightHide(float speed)
{
    action = SLIDERIGHTHIDE;
    widthInc = currentWidth / speed;
    reset(speed);
} // end slideHideDown method

void UIEntity::slideRightShow(float speed)
{
    isHidden = false;
    action = SLIDERIGHTSHOW;
    widthInc = (endWidth - currentWidth) / speed;
    reset(speed);
} // end slideShowDown method

void UIEntity::toggleRightSlide(float speed)
{
    if (isHidden || action == SLIDERIGHTHIDE) {
        action = SLIDERIGHTSHOW;
        isHidden = false;
        widthInc = (endWidth - currentWidth) / speed;
    } else {
        action = SLIDERIGHTHIDE;
        widthInc = currentWidth / speed;
    }
    reset(speed);
} // end toggleSlide method

void UIEntity::fadeOut(float speed)
{
    action = FADEOUT;
    alphaInc = currentAlpha / speed;
    reset(speed);
} // end fadeOut method

void UIEntity::fadeIn(float speed)
{
    isHidden = false;
    action = FADEIN;
    alphaInc = (endAlpha - currentAlpha) / speed;
    reset(speed);
} // end fadeIn method

void UIEntity::toggleFade(float speed)
{
    if (isHidden || action == FADEOUT) {
        action = FADEIN;
        isHidden = false;
        alphaInc = (endAlpha - currentAlpha) / speed;
    } else {
        action = FADEOUT;
        alphaInc = currentAlpha / speed;
    }
    reset(speed);
} // end toggleFade method

void UIEntity::setPos(Vect pos)
{
    this->pos = pos;
} // end setPos method

void UIEntity::setSize(Vect size)
{
    currentWidth = size.x;
    currentHeight = size.y;
} // end setSize method

void UIEntity::setAlpha(float alpha)
{
    currentAlpha = alpha;
} // end setAlpha method

void UIEntity::setToolTip(UIEntity *toolTip)
{
    this->toolTip = toolTip;
} // end setTooLTip method

void UIEntity::setDefaultTexture(char *filename)
{
    loadTexture(filename, defaultTexture);
    texture = defaultTexture;
} // end setTexture method

void UIEntity::setDepressedTexture(char *filename)
{
    filename = strcat("Media/", filename);
    loadTexture(filename, depressedTexture);
} // end setTexture method

Vect UIEntity::getPos()
{
    return pos;
} // end getPos method

Vect UIEntity::getSize()
{
    return Vect(currentWidth, currentHeight); /** Make currentWidth, currentHeight a Vect size **/
} // end getSize method

float UIEntity::getAlpha()
{
    return currentAlpha;
} // end getAlpha method

bool UIEntity::checkCollide(Vect pos)
{
    return (pos.x >= this->pos.x && pos.x <= this->pos.x + currentWidth && pos.y >= this->pos.y && pos.y <= this->pos.y + currentHeight);
} // end checkCollide method

void UIEntity::draw(Vect pos, float alpha)
{
    if (!isHidden) {
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (texture != NULL) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture);
            glColor4f(1.0f, 1.0f, 1.0f, alpha * currentAlpha);
            glBegin(GL_QUADS);
                glTexCoord2i(0, 0);
                glVertexVect(pos);

                glTexCoord2i(1, 0);
                glVertex2f(pos.x + currentWidth, pos.y);

                glTexCoord2i(1, 1);
                glVertex2f(pos.x + currentWidth, pos.y + currentHeight);

                glTexCoord2i(0, 1);
                glVertex2f(pos.x, pos.y + currentHeight);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
        glColor(color);
        glBegin(GL_QUADS);
            glVertexVect(pos);
            glVertex2f(pos.x + currentWidth, pos.y);
            glVertex2f(pos.x + currentWidth, pos.y + currentHeight);
            glVertex2f(pos.x, pos.y + currentHeight);
        glEnd();
        glBlendFunc (GL_SRC_ALPHA, GL_ONE);
        if (toolTip != NULL) {
            toolTip->draw(pos + toolTip->getPos(), toolTip->getAlpha());
        }
        altDraw(pos, alpha);
    }
} // end draw method

void UIEntity::step(Vect pos)
{
    if (!isHidden) {
        if (!isClicked) {
            if (isHovered) {
                if(!checkCollide(pos)) {
                    offHover(pos);
                }
            } else {
                if (checkCollide(pos)) {
                    onHover(pos);
                }
            }
        } else {
            if (!checkCollide(pos)) {
                onRelease(pos);
            }
        }
        if (endSteps != 0) {
            if (currentSteps == endSteps) {
                if (action % 2 == 0 && action < 12) {
                    isHidden = true;
                } else {
                    isHidden = false;
                }
                action = NONE;
                reset(0);
            }
            int stepType = floorf(action / 2);
            if (stepType < 6) {
                if (stepType == 0) {
                    hideStep();
                } else if (stepType == 1) {
                    slideUpStep();
                } else if (stepType == 2) {
                    slideDownStep();
                } else if (stepType == 3) {
                    slideLeftStep();
                } else if (stepType == 4) {
                    slideRightStep();
                } else if (stepType == 5) {
                    fadeStep();
                }
                currentSteps += 1;
            }
        }
        altStep(pos);
        if (toolTip != NULL)
            toolTipStep(pos);
    }
} // end step method

void UIEntity::toolTipStep(Vect pos)
{
    if (checkCollide(pos) && toolTip->getAlpha() <= 0.0f) {
        if (pos == hoverPos) {
            hoverTime += 1;
        } else {
            hoverTime = 0;
        }
        if (hoverTime == 20) {
            toolTip->fadeIn(30);
            toolTip->setPos(pos);
        }
        hoverPos = pos;
    }
    toolTip->step(pos);
} // end toolTipStep method

void UIEntity::onClick(Vect pos)
{
    isClicked = true;
    isHovered = false;
    onClickAction(pos);
} // end onClick method

void UIEntity::onRelease(Vect pos)
{
    isClicked = false;
    onReleaseAction(pos);
} // end onRelease method

void UIEntity::onHover(Vect pos)
{
    isHovered = true;
    onHoverAction(pos);
} // end onHover method

void UIEntity::offHover(Vect pos)
{
    isHovered = false;
    offHoverAction(pos);
} // end offHover method

Button::Button(Vect pos, float width, float height, float alpha, bool isHidden, UIAction hideAction) :
    UIEntity(pos, width, height, alpha, isHidden, hideAction)
{
} // end constructor

void Button::addEntity(UIEntity *actionEntity, UIAction action, float speed)
{
    actionEntities.push_back(actionEntity);
    actions.push_back(action);
    speeds.push_back(speed);
} // end addEntity method

void Button::addEntity(UIEntity *actionEntity, UIAction action)
{
    actionEntities.push_back(actionEntity);
    actions.push_back(action);
    speeds.push_back(1);
} // end addEntity method

void Button::altDraw(Vect pos, float alpha)
{
} // end draw method

void Button::altStep(Vect pos)
{
} // end altStep method

void Button::onClickAction(Vect pos)
{
    for (unsigned int i = 0; i < actionEntities.size(); i += 1) {
        switch(actions[i]) {
            case HIDE:
                actionEntities[i]->hide(speeds[i]); /** Figure out how to pass functions as arguments **/
                break;
            case SHOW:
                actionEntities[i]->show(speeds[i]);
                break;
            case SLIDEUPHIDE:
                actionEntities[i]->slideUpHide(speeds[i]);
                break;
            case SLIDEUPSHOW:
                actionEntities[i]->slideUpShow(speeds[i]);
                break;
            case SLIDEDOWNHIDE:
                actionEntities[i]->slideDownHide(speeds[i]);
                break;
            case SLIDEDOWNSHOW:
                actionEntities[i]->slideDownShow(speeds[i]);
                break;
            case SLIDELEFTHIDE:
                actionEntities[i]->slideLeftHide(speeds[i]);
                break;
            case SLIDELEFTSHOW:
                actionEntities[i]->slideLeftShow(speeds[i]);
                break;
            case SLIDERIGHTHIDE:
                actionEntities[i]->slideRightHide(speeds[i]);
                break;
            case SLIDERIGHTSHOW:
                actionEntities[i]->slideRightShow(speeds[i]);
                break;
            case FADEOUT:
                actionEntities[i]->fadeOut(speeds[i]);
                break;
            case FADEIN:
                actionEntities[i]->fadeIn(speeds[i]);
                break;
            case TOGGLE:
                actionEntities[i]->toggle(speeds[i]);
                break;
            case TOGGLEUPSLIDE:
                actionEntities[i]->toggleUpSlide(speeds[i]);
                break;
            case TOGGLEDOWNSLIDE:
                actionEntities[i]->toggleDownSlide(speeds[i]);
                break;
            case TOGGLELEFTSLIDE:
                actionEntities[i]->toggleLeftSlide(speeds[i]);
                break;
            case TOGGLERIGHTSLIDE:
                actionEntities[i]->toggleRightSlide(speeds[i]);
                break;
            case TOGGLEFADE:
                actionEntities[i]->toggleFade(speeds[i]);
                break;
        }
    } // end for (i)
    color = Color(0.0f, 0.0f, 0.0f, 0.3f);
} // end onClick method

void Button::onReleaseAction(Vect pos)
{
    color = Color(0.0f, 0.0f, 0.0f, 0.0f);
} // end onClick method

void Button::onHoverAction(Vect pos)
{
    color = Color(1.0f, 1.0f, 1.0f, 0.2f);
} // end onHover method

void Button::offHoverAction(Vect pos)
{
    color = Color(0.0f, 0.0f, 0.0f, 0.0f);
} // end offHover method

Container::Container(Vect pos, float width, float height, float alpha, bool isHidden, UIAction hideAction, bool hasAbsolutePos) :
    UIEntity(pos, width, height, alpha, isHidden, hideAction)
{
    this->hasAbsolutePos = hasAbsolutePos;
} // end constructor

Container::~Container()
{
    for(std::vector<UIEntity*>::iterator i = items.begin(); i < items.end(); i += 1) {
        delete (*i);
    } // end for (i)
} // end destructor

void Container::setPadding(const Vect &padding)
{
    this->padding = padding;
} // end setPadding method

void Container::addEntity(UIEntity *item)
{
    items.push_back(item);
} // end addListItem method

void Container::altDraw(Vect pos, float alpha)
{
    if (hasAbsolutePos) {
        for(std::vector <UIEntity *>::iterator i = items.begin(); i < items.end(); i += 1) {
            (*i)->draw(pos + (*i)->getPos(), currentAlpha * alpha);
        } // end for (i)
    } else {
        Vect offset(0, 0);
        for(std::vector <UIEntity *>::iterator i = items.begin(); i < items.end(); i += 1) {
            if (offset.x + (*i)->getSize().x < this->currentWidth) {
                (*i)->draw(pos + offset + padding, alpha * currentAlpha);
            } else {
                offset = Vect(0, offset.y + (*i)->getSize().y + padding.y);
                (*i)->draw(pos + offset + padding, alpha * currentAlpha);
            }
            offset += Vect((*i)->getSize().x + padding.x, 0);
        } // end for (i)
    }
} // end draw method

void Container::altStep(Vect pos)
{
    if (hasAbsolutePos) {
        for (std::vector <UIEntity *>::iterator i = items.begin(); i < items.end(); i += 1) {
            (*i)->step(pos - this->pos);
        } // end for (i)
    } else {
        Vect offset(0, 0);
        for(std::vector <UIEntity *>::iterator i = items.begin(); i < items.end(); i += 1) {
            if (offset.x + (*i)->getSize().x < this->currentWidth) {
                (*i)->step(pos + (*i)->getPos() - offset - padding - this->pos);
            } else {
                offset = Vect(0, offset.y + (*i)->getSize().y + padding.y);
                (*i)->step(pos + (*i)->getPos() - offset - padding - this->pos);
            }
            offset += Vect((*i)->getSize().x + padding.x, 0);
        } // end for (i)
    }
} // end altStep method

void Container::onClickAction(Vect pos)
{
    if (!isHidden) {
        if (hasAbsolutePos) {
            for (std::vector <UIEntity *>::iterator i = items.begin(); i < items.end(); i += 1) {
                if ((*i)->checkCollide(pos - this->pos)) {
                    (*i)->onClick(pos - this->pos);
                }
            } // end for (i)
        } else {
            Vect offset(0, 0);
            for(std::vector <UIEntity *>::iterator i = items.begin(); i < items.end(); i += 1) {
                if (offset.x + (*i)->getSize().x < this->currentWidth) {
                    if ((*i)->checkCollide(pos + (*i)->getPos() - offset - padding - this->pos)) {
                        (*i)->onClick(pos + (*i)->getPos() - offset - padding - this->pos);
                    }
                } else {
                    offset = Vect(0, offset.y + (*i)->getSize().y + padding.y);
                    if ((*i)->checkCollide(pos + (*i)->getPos() - offset - padding - this->pos)) {
                        (*i)->onClick(pos + (*i)->getPos() - offset - padding - this->pos);
                    }
                }
                offset += Vect((*i)->getSize().x + padding.x, 0);
            } // end for (i)
        }
    }
} // end onClick method

void Container::onReleaseAction(Vect pos)
{
    if (hasAbsolutePos) {
        for (std::vector <UIEntity *>::iterator i = items.begin(); i < items.end(); i += 1) {
            if ((*i)->checkCollide(pos - this->pos)) {
                (*i)->onRelease(pos - this->pos);
            }
        } // end for (i)
    } else {
        Vect offset(0, 0);
        for(std::vector <UIEntity *>::iterator i = items.begin(); i < items.end(); i += 1) {
            if (offset.x + (*i)->getSize().x < this->currentWidth) {
                if ((*i)->checkCollide(pos + (*i)->getPos() - offset - padding - this->pos)) {
                    (*i)->onRelease(pos + (*i)->getPos() - offset - padding - this->pos);
                }
            } else {
                offset = Vect(0, offset.y + (*i)->getSize().y + padding.y);
                if ((*i)->checkCollide(pos + (*i)->getPos() - offset - padding - this->pos)) {
                    (*i)->onRelease(pos + (*i)->getPos() - offset - padding - this->pos);
                }
            }
            offset += Vect((*i)->getSize().x + padding.x, 0);
        } // end for (i)
    }
} // end onRelease method

void Container::onHoverAction(Vect pos)
{
} // end onHover method

void Container::offHoverAction(Vect pos)
{
} // end offHover method

ToolTip::ToolTip(float width, float height, float alpha, bool isHidden, UIAction hideAction) :
    UIEntity(Vect(0, 0), width, height, alpha, isHidden, hideAction)
{
} // end constructor

void ToolTip::altDraw(Vect pos, float alpha)
{
} // end draw method

void ToolTip::altStep(Vect pos)
{
} // end altStep method

void ToolTip::onClickAction(Vect pos)
{
} // end onClick method

void ToolTip::onReleaseAction(Vect pos)
{
} // end onClick method

void ToolTip::onHoverAction(Vect pos)
{
} // end onHover method

void ToolTip::offHoverAction(Vect pos)
{
    fadeOut(30);
} // end offHover method

PercentageBar::PercentageBar(Vect pos, float width, float height, float alpha, bool isHidden, float *attribute, float attributeMax, UIAction hideAction) :
    UIEntity(pos, width, height, alpha, isHidden, hideAction)
{
    this->attribute = attribute;
    this->attributeMax = attributeMax;
} // end constructor

void PercentageBar::altDraw(Vect pos, float alpha)
{
    float percentage = (*attribute) / attributeMax;
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, alpha * currentAlpha);
    glBegin(GL_QUADS); /** Add Texture related code **/
        glVertex2f(pos.x + endWidth * percentage, pos.y);
        glVertex2f(pos.x + endWidth, pos.y);
        glVertex2f(pos.x + endWidth, pos.y + currentHeight);
        glVertex2f(pos.x + endWidth * percentage, pos.y + currentHeight);
    glEnd();
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);
} // end draw method

void PercentageBar::altStep(Vect pos)
{
} // end altStep method

void PercentageBar::onClickAction(Vect pos)
{
} // end onClick method

void PercentageBar::onReleaseAction(Vect pos)
{
} // end onClick method

void PercentageBar::onHoverAction(Vect pos)
{
} // end onHover method

void PercentageBar::offHoverAction(Vect pos)
{
} // end offHover method

ScrollBar::ScrollBar(UIEntity *partOf, UIAction hideAction) :
    UIEntity(partOf->getPos(), partOf->getSize().x, partOf->getSize().y, partOf->getAlpha(), false, hideAction)
{
    this->toolTip = partOf; /** REMOVE **/
} // end constructor

void ScrollBar::altDraw(Vect pos, float alpha)
{
} // end draw method

void ScrollBar::altStep(Vect pos)
{
} // end altStep method

void ScrollBar::onClickAction(Vect pos)
{
} // end onClick method

void ScrollBar::onReleaseAction(Vect pos)
{
} // end onClick method

void ScrollBar::onHoverAction(Vect pos)
{
} // end onHover method

void ScrollBar::offHoverAction(Vect pos)
{
} // end offHover method

Timer::Timer(Vect pos, float width, float height, float alpha, bool isHidden, UIAction hideAction) :
    UIEntity(pos, width, height, alpha, isHidden, hideAction)
{
    isPaused = false;
    turnLength = 1000 * 60; /** 60 seconds **/
    resetTimer();
    timerFont.Create("timer.glf", timerTexture);
} // end constructor

void Timer::startTimer()
{
    startTime = SDL_GetTicks();
    endTime = startTime + turnLength;
} // end startTime method

void Timer::pauseTimer()
{
} // end stopTime method

void Timer::togglePause()
{
    if (isPaused) {
        isPaused = false;
        startTimer();
    } else {
        isPaused = true;
        pauseTimer();
    }
} // end pause method

void Timer::altDraw(Vect pos, float alpha)
{
    std::ostringstream oss;
    Uint32 timeLeft = (turnLength - (SDL_GetTicks() - startTime));
    Uint32 minutes = ceil(timeLeft / 60000);
    Uint32 seconds = ceil((timeLeft / 1000) % 60);
    oss << minutes << ":";
    if (seconds < 10) {
        oss << 0;
    }
    oss << seconds;
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, alpha * currentAlpha);
    timerFont.Begin();
        timerFont.textOut(oss.str(), pos.x + currentWidth / 2 - 12, pos.y + currentHeight / 2 - 7, 0.0f); /** centering not finished **/
    glEnd();
    glDisable(GL_TEXTURE_2D);
} // end draw method

void Timer::altStep(Vect pos)
{
} // end altStep method

void Timer::onClickAction(Vect pos)
{
} // end onClick method

void Timer::onReleaseAction(Vect pos)
{
} // end onClick method

void Timer::onHoverAction(Vect pos)
{
} // end onHover method

void Timer::offHoverAction(Vect pos)
{
} // end offHover method

void Timer::resetTimer()
{
    startTime = SDL_GetTicks();
    endTime = startTime + turnLength;
} // end resetTimer method

bool Timer::checkTimer()
{
    if (SDL_GetTicks() >= endTime) {
        return true;
    }
    return false;
} // end stepTimer method

TextArea::TextArea(Vect pos, float width, float height, float alpha, bool isHidden, UIAction hideAction) :
    UIEntity(pos, width, height, alpha, isHidden, hideAction)
{
    text = "";
    textFont.Create("text.glf", textTexture); /** Change font possibly **/
} // end constructor

void TextArea::setText(std::string text)
{
    this->text = text;
} // end setText method

void TextArea::altDraw(Vect pos, float alpha)
{
    float widthPadding = (currentWidth - text.length() * 9) / 2;
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, alpha * currentAlpha);
    textFont.Begin();
        textFont.textOut(text, pos.x + widthPadding, pos.y , 0.0f); /** centering might not be finished **/
    glEnd();
    glDisable(GL_TEXTURE_2D);
} // end draw method

void TextArea::altStep(Vect pos)
{
} // end altStep method

void TextArea::onClickAction(Vect pos)
{
} // end onClick method

void TextArea::onReleaseAction(Vect pos)
{
} // end onClick method

void TextArea::onHoverAction(Vect pos)
{
} // end onHover method

void TextArea::offHoverAction(Vect pos)
{
} // end offHover method

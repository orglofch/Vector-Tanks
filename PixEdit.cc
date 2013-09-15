#include "PixEdit.h"
#include "PixMap.h"
#include "PixNode.h"

PixEdit::PixEdit(PixMap * pixMap, Vect pos)
{
    this->pos = pos;
    this->pixMap = pixMap;
} // end constructor

std::pair <int, int> PixEdit::getXBound(float pos, float radius)
{
    std::pair <int, int> bound;
    if (pos - radius < 0) {
        bound.first = 0;
        bound.second = pos + radius;
    } else if (pos + radius >= mapWidth) {
        bound.first = pos - radius;
        bound.second = mapWidth - 1;
    } else {
        bound.first = pos - radius;
        bound.second = pos + radius;
    }
    return bound;
} // end getXBound method

std::pair <int, int> PixEdit::getYBound(float pos, float radius)
{
    std::pair <int, int> bound;
    if (pos - radius < 0) {
        bound.first = 0;
        bound.second = pos + radius;
    } else if (pos + radius >= mapHeight) {
        bound.first = pos - radius;
        bound.second = mapHeight - 1;
    } else {
        bound.first = pos - radius;
        bound.second = pos + radius;
    }
    return bound;
} // end getYBound method

RemoveCircle::RemoveCircle(PixMap *pixMap, Vect pos, int radius) :
    PixEdit (pixMap, pos)
{
    shakeCounter = 0;
    shakeAngle = rand() % 361;
    endRadius = radius;
    currentRadius = 0;

    color = Color(0.3f, 0.3f, 0.9f, 0.1f);
} // end constructor

void RemoveCircle::draw()
{
    for (int i = currentRadius; i > 0; i -= 1) {
        glColor(color);
        glVertexVect(pos);
        glBegin(GL_TRIANGLE_FAN);
            for (int angle = 0; angle <= 360; angle += 5) {
                glVertex2f(pos.x + sin(degreetoRad(angle)) * i, pos.y + cos(degreetoRad(angle)) * i);
            } // end for (angle)
        glEnd();
    } // end for (i)
    color.alpha -= 0.01;
} // end draw method

bool RemoveCircle::step()
{
    /** Shake add back later possibly if (shakeCounter % 2 == 0) {
        glTranslatef(cos(degreetoRad(shakeAngle)) * endRadius / 6, sin(degreetoRad(shakeAngle)) * endRadius / 6, 0.0f);
    } else {
        glTranslatef(-cos(degreetoRad(shakeAngle)) * endRadius / 6, -sin(degreetoRad(shakeAngle)) * endRadius / 6, 0.0f);
        shakeAngle = rand() % 361;
    } **/
    shakeCounter += 1;

    std::pair <int, int> boundX = getXBound(pos.x, currentRadius);
    std::pair <int, int> boundY = getYBound(pos.y, currentRadius);

    float velo, angle;
    for (int i = boundX.first; i <= boundX.second; i += 1) {
        for (int j = boundY.first; j <= boundY.second; j += 1) {
            if (currentRadius <= endRadius) {
                if(distance(pos, {i, j}) <= currentRadius && pixMap->checkPixCollide({i, j})) {
                    if ((i + j) % 3 == 0) {
                        velo = (float)rand() / (float)RAND_MAX;
                        angle = atan2((pos.y - j), (i - pos.x));
                        Particle *newParticle = new LineParticle(pixMap, {i, j}, {cos(angle) * velo, sin(angle) * velo}, 100, true, false);
                        newParticle->setColor(pixMap->getPixNode({i, j})->getCol());
                        pEng.pushParticle(newParticle);
                    }
                    pixMap->deletePixNode({i, j});
                }
            } else {
                return true;
            }
        } // end for (j)
    } // end for (i)
    currentRadius += 1;
    return false;
} // end step method

void RemoveCircle::onFinish()
{
    /** if (shakeCounter % 2 == 1)
        glTranslatef(-cos(degreetoRad(shakeAngle)) * endRadius / 6, -sin(degreetoRad(shakeAngle)) * endRadius / 6, 0.0f); **/
} // end onFinish method

CollapseCircle::CollapseCircle(PixMap *pixMap, Vect pos, int radius) :
    PixEdit (pixMap, pos)
{
    endRadius = radius;

    colBottom = NULL;
} // end constructor

void CollapseCircle::findBottoms()
{
    std::pair <int, int> boundX = getXBound(pos.x, endRadius);

    colBottom = new int [endRadius * 2 + 1]; /** make proper size **/
    for (int i = boundX.first; i <= boundX.second; i += 1) {
        int tempY = pos.y;
        while (!pixMap->checkPixCollide({i, tempY}) && distance(pos, {i, tempY}) <= endRadius + 1 && tempY > 0)
        {
            tempY -= 1;
        } // end while
        if (distance(pos, {i, tempY}) > endRadius + 1 || tempY <= 0 || tempY >= mapHeight - 1) {
            colBottom [i - boundX.first] = NULL;
        } else {
            colBottom [i - boundX.first] = tempY + 1;
        }
    } // end for (i)
} // end findBottoms method

void CollapseCircle::draw()
{

} // end draw method

bool CollapseCircle::step()
{
    std::pair <int, int> boundX = getXBound(pos.x, endRadius);

    if (colBottom == NULL) {
        findBottoms(); // Find the bottom positions on the first run through
    }
    bool isDone = true; // Whether or not anything is still falling
    for (int i = boundX.first; i <= boundX.second; i += 1) {
        if (colBottom [i - boundX.first] != NULL) {
            if (!pixMap->checkPixCollide({i, colBottom [i - boundX.first]})) {
                isDone = false;
                int tempY = colBottom [i - boundX.first] - 1;
                do {
                    pixMap->swapNodes({i, tempY + 1}, {i, tempY});
                    tempY -= 1;
                } while (pixMap->checkPixCollide({i, tempY}) && tempY >= 0);
                if (colBottom [i - boundX.first] < mapHeight - 1)
                    colBottom [i - boundX.first] += 1;
                else
                    colBottom [i - boundX.first] = NULL;
            } else {
                Particle *newParticle = new LineParticle(pixMap, {i, colBottom [i - boundX.first]}, {(float)rand() / (float)RAND_MAX - 0.5, (float)rand() / (float)RAND_MAX - 0.9}, 70, true, false);
                newParticle->setColor(pixMap->getPixNode({i, colBottom [i - boundX.first]})->getCol());
                pEng.pushParticle(newParticle);
                colBottom [i - boundX.first] = NULL;
            }
        }
    } // end for (i)
    return isDone;
} // end collapse method

void CollapseCircle::onFinish()
{

} // end onFinish method

BuildSpike::BuildSpike(PixMap *pixMap, Vect pos, int height, int radius) :
    PixEdit(pixMap, pos)
{
    heightInc = height / radius;
    this->radius = radius;
    currentHeight = 0;
    currentWidth = 0;
    buildCounter = 0;
} // end constructor

void BuildSpike::draw()
{
} // end draw method

bool BuildSpike::step()
{
    if (currentWidth <= radius) {
        if (buildCounter % 4 == 0) {
            float currentDist = currentWidth;
            for (int i = pos.y; i >= pos.y -(currentHeight + 1); i -= 1) {
                Particle *newParticle;
                if (!pixMap->checkPixCollide({pos.x - currentDist, i})) {
                    pixMap->createPixNode({pos.x - currentDist, i}, DIRT, {0.5f, 0.5f, 0.2f, 1.0f});
                    newParticle = new CircleParticle(pixMap, Vect(pos.x - currentDist, i), Vect(0, 0), 10, true, false);
                    pEng.pushParticle(newParticle);
                }
                if (!pixMap->checkPixCollide({pos.x + currentDist, i})) {
                    pixMap->createPixNode({pos.x - currentDist, i}, DIRT, {0.5f, 0.5f, 0.2f, 1.0f});
                    newParticle = new CircleParticle(pixMap, Vect(pos.x + currentDist, i), Vect(0, 0), 10, true, false);
                    pEng.pushParticle(newParticle);
                }
                currentDist -= 1 / heightInc;
            } // end for (i)
            for (int i = pos.y - buildCounter; i >= pos.y - currentHeight; i -= 1) { /** Doesn't work yet **/
                if (!pixMap->checkPixCollide({pos.x, i})) {
                    pixMap->createPixNode({pos.x - currentDist, i}, DIRT, {0.5f, 0.5f, 0.2f, 1.0f});
                }
            } // end for (i)
            currentWidth += 1;
            currentHeight += heightInc;
        }
        buildCounter += 1;
        return false;
    }
    return true;
} // end step method

void BuildSpike::onFinish()
{
    pixMap->pushCollapse({pos.x, pos.y + 1}, radius);
} // end onFinishmethod

LiquifyCircle::LiquifyCircle(PixMap *pixMap, Vect pos, int radius) :
    PixEdit (pixMap, pos)
{
    endRadius = radius;
    currentRadius = 0;
} // end constructor

void LiquifyCircle::draw()
{
} // end draw method

bool LiquifyCircle::step()
{
    std::pair <int, int> boundX = getXBound(pos.x, currentRadius);
    std::pair <int, int> boundY = getYBound(pos.y, currentRadius);

    for (int i = boundX.first; i <= boundX.second; i += 1) {
        for (int j = boundY.first; j <= boundY.second; j += 1) {
            if (currentRadius <= endRadius) {
                if(distance(pos, {i, j}) <= currentRadius && pixMap->checkPixCollide({i, j}) && !pixMap->checkPixType({i, j}, WATER)) {
                    pixMap->deletePixNode({i, j});
                    pixMap->createPixNode({i, j}, WATER, {0.4f, 0.3f, 0.7f, 0.6f});
                    pixMap->pushActivePixNode({i, j});
                }
            } else {
                return true;
            }
        } // end for (j)
    } // end for (i)
    currentRadius += 1;
    return false;
} // end step method

void LiquifyCircle::onFinish()
{
} // end onFinish method

BuildWall::BuildWall(PixMap *pixMap, Vect pos, int width, int height) :
    PixEdit (pixMap, pos)
{
    endHeight = height;
    currentHeight = 0;
    this->width = width;
} // end constructor

void BuildWall::draw()
{
} // end draw method

bool BuildWall::step()
{
    if (pos.y - currentHeight >= 0 && currentHeight != endHeight) {
        std::pair <int, int> boundX = getXBound(pos.x - width / 2, width);

        for (int i = boundX.first; i <= boundX.second; i += 1) {
            if(!pixMap->checkPixCollide({i, pos.y - currentHeight})) {
                float angle = rand() % 2 * 180;
                float distance = (float)rand() / (float)RAND_MAX * 20 + 5;
                Vect startPos = Vect(i, pos.y - currentHeight) + createVect(angle, distance);
                Particle *newParticle = new PixNodeParticle(pixMap, (int)DIRT, startPos, Vect(0, 0), Vect(i, pos.y - currentHeight), true, false);
                newParticle->setColor({0.4f, 0.7f, 0.3f, 1.0f});
                pEng.pushParticle(newParticle);
            }
        } // end for (i)
        currentHeight += 1;
        return false;
    }
    return true;
} // end step method

void BuildWall::onFinish()
{
    pixMap->pushCollapse({pos.x - width / 2 - 1, pos.y + 1}, width + 1); /** Not all of the particles are done settling when this is called **/
} // end onFinish method

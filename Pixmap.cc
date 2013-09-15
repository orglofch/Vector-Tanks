#include "PixMap.h"

#include "PixNode.h"
#include "PixEdit.h"
#include "Player.h"
#include "Weapons.h"

#include <iostream>

PixMap::PixMap()
{
    pixNodes = new PixNode ** [mapWidth];
    for (int i = 0; i < mapWidth; i += 1) {
        pixNodes [i] = new PixNode * [mapHeight];
        for (int j = 0; j < mapHeight; j += 1) {
            pixNodes[i] [j] = NULL;
        } // end for (j)
    } // end for (i)
    activePixNodes = new std::vector<PixNode*> [mapHeight];
} // end constructor

PixMap::~PixMap()
{
    for (int i = 0; i < mapWidth; i += 1) {
        for (int j = 0; j < mapHeight; j += 1) {
            if (pixNodes [i] [j] != NULL) {
                delete pixNodes[i][j];
            }
        } // end for (j)
        delete [] pixNodes[i];
    } // end for (i)
    delete [] pixNodes;
    for (std::vector <PixEdit*>::iterator i = activeEditSections.begin(); i < activeEditSections.end(); i += 1) {
        delete *i;
    } // end for (i)
    for (std::vector <Projectile*>::iterator i = activeProjectiles.begin(); i < activeProjectiles.end(); i += 1) {
        delete *i;
    } // end for (i)
    for (std::vector <Projectile*>::iterator i = stackProjectiles.begin(); i < stackProjectiles.end(); i += 1) {
        delete *i;
    } // end for (i)
} // end destructor

void PixMap::drawEditSections()
{
    for (std::vector <PixEdit*>::iterator i = activeEditSections.begin(); i < activeEditSections.end(); i += 1) {
        (*i)->draw();
    } // end for (i)
} // end drawEditSections method

void PixMap::drawProjectiles()
{
    for (std::vector <Projectile*>::iterator i = activeProjectiles.begin(); i < activeProjectiles.end(); i += 1) {
        (*i)->draw();
    } // end for (i)
} // end stepEditSections

void PixMap::stepEditSections()
{
    std::vector<int> removeIndices;
    int offset = 0;
    for (std::vector <PixEdit*>::iterator i = activeEditSections.begin(); i < activeEditSections.end(); i += 1) {
        if ((*i)->step()) {
            (*i)->onFinish();
            removeIndices.push_back(i - activeEditSections.begin());
           /** delete *i; Causes segfaults for some reason **/
        }
    } // end for (i)
    for (std::vector<int>::iterator i = removeIndices.begin(); i < removeIndices.end(); i += 1) {
        activeEditSections.erase(activeEditSections.begin() + (*i) - offset);
        offset += 1;
    } // end for (i)
} // end stepRemove method

void PixMap::stepProjectiles()
{
    std::vector<int> removeIndices;
    int offset = 0;
    for (std::vector <Projectile*>::iterator i = activeProjectiles.begin(); i < activeProjectiles.end(); i += 1) {
        if ((*i)->step()) {
            removeIndices.push_back(i - activeProjectiles.begin());
            /** create deleter **/
        }
    } // end for (i)
    for (std::vector<int>::iterator i = removeIndices.begin(); i < removeIndices.end(); i += 1) {
        activeProjectiles.erase(activeProjectiles.begin() + (*i) - offset);
        offset += 1;
    } // end for (i)
} // end stepProjectiles

void PixMap::stepPixNodes()
{
    for (int i = mapHeight - 1; i >= 0; i -= 1) {
        for (std::vector <PixNode*>::iterator j = activePixNodes[i].begin(); j < activePixNodes[i].end(); j += 1) {
            Vect pos = (*j)->getPos();
            (*j)->step();
            (*j)->makeInactive();
            /** create deleter **/
        } // end for (j)
    } // end for (i)
    for (int i = 0; i < mapHeight; i += 1) {
        activePixNodes[i].clear();
    } // end for (i)
} // end stepPixNodes method

void PixMap::unloadEditSectionStack()
{
    for (std::vector <PixEdit*>::iterator i = stackEditSections.begin(); i < stackEditSections.end(); i += 1) {
        activeEditSections.push_back(*i);
    } // end for (i)
    stackEditSections.clear();
} // end unloadEditSectionStack method

void PixMap::unloadProjectileStack()
{
    for (std::vector <Projectile*>::iterator i = stackProjectiles.begin(); i < stackProjectiles.end(); i += 1) {
        activeProjectiles.push_back(*i);
    } // end for (i)
    stackProjectiles.clear();
} // end unloadProjectileStack method

void PixMap::unloadPixNodeStack()
{
    for (std::vector <PixNode*>::iterator i = stackPixNodes.begin(); i < stackPixNodes.end(); i += 1) {
        Vect pos = (*i)->getPos();
        if (!checkPixActive(pos)) {
            PixNode *node = getPixNode(pos);
            if (node->canActive({pos.x, pos.y + 1}) || node->canActive({pos.x + 1, pos.y}) || node->canActive({pos.x - 1, pos.y})) {
                (*i)->makeActive();
                activePixNodes[(int)pos.y].push_back(*i);
            }
        }
    } // end for (i)
    stackPixNodes.clear();
} // end unloadPixNodeStack

void PixMap::deletePixNode(Vect pos)
{
    std::vector<int> removeIndices;
    int offset = 0;
    for(std::vector<PixNode*>::iterator i = stackPixNodes.begin(); i < stackPixNodes.end(); i += 1) {
        if (pos == (*i)->getPos()) {
            removeIndices.push_back(i - stackPixNodes.begin());
        }
    } // end for (i)
    for (std::vector<int>::iterator i = removeIndices.begin(); i < removeIndices.end(); i += 1) {
        stackPixNodes.erase(stackPixNodes.begin() + (*i) - offset);
        offset += 1;
    } // end for (i)
    std::vector<std::pair <int, int> > activeRemoveIndices;
    offset = 0;
    for (int j = 0; j < mapHeight; j += 1) {
        for(std::vector<PixNode*>::iterator i = activePixNodes[j].begin(); i < activePixNodes[j].end(); i += 1) {
            if (pos == (*i)->getPos()) {
                activeRemoveIndices.push_back({j, i - activePixNodes[j].begin()});
            }
        } // end for (i)
    } // end for (j)
    for (std::vector<std::pair <int, int> >::iterator i = activeRemoveIndices.begin(); i < activeRemoveIndices.end(); i += 1) {
        activePixNodes[i->first].erase(activePixNodes[i->first].begin() + i->second - offset);
        offset += 1;
    } // end for (i)

    delete getPixNode(pos);
    pixNodes[(int)pos.x][(int)pos.y] = NULL;
    tripActives(pos);
} // end deletePixNode method

void PixMap::createPixNode(Vect pos, int type, Color color)
{
    if (getPixNode(pos) == NULL) {
        PixNode *newPixNode;
        if (type == DIRT) {
            newPixNode = new Dirt();
        } else if (type == SAND) {
            newPixNode = new Sand();
        } else if (type == WATER) {
            newPixNode = new Water();
        } else if (type == LAVA) {
            newPixNode = new Lava();
        } else if (type == SNOW) {
            newPixNode = new Snow();
        }
        newPixNode->setPos(pos);
        newPixNode->setPixMap(this);
        newPixNode->setCol(color);
        pixNodes[(int)pos.x][(int)pos.y] = newPixNode;
        pushActivePixNode(pos);
    }
} // end createPixNode method

void PixMap::pushRemove(Vect pos, int radius)
{
    PixEdit *newEdit = new RemoveCircle(this, pos, radius);
    stackEditSections.push_back(newEdit);
} // end pushRemoveSection method

void PixMap::pushCollapse(Vect pos, int radius)
{
    PixEdit *newEdit = new CollapseCircle(this, pos, radius);
    stackEditSections.push_back(newEdit);
} // end pushCollapseSection method

void PixMap::pushBuild(Vect pos, int width, int height)
{
    PixEdit *newEdit = new BuildWall(this, pos, width, height);
    stackEditSections.push_back(newEdit);
} // end pushBuild method

void PixMap::pushLiquify(Vect pos, int height)
{
    PixEdit *newEdit = new LiquifyCircle(this, pos, height);
    stackEditSections.push_back(newEdit);
} // end pushBuild method

void PixMap::pushActivePixNode(Vect pos)
{
    if (getPixNode(pos) != NULL) {
        PixNode *node = getPixNode(pos);
        if (node->canActive({pos.x, pos.y + 1}) || node->canActive({pos.x + 1, pos.y}) || node->canActive({pos.x - 1, pos.y})) {
            stackPixNodes.push_back(getPixNode(pos));
        }
    }
} // end pushActivePixNode

void PixMap::pushProjectile(Projectile *newProjectile)
{
    stackProjectiles.push_back(newProjectile);
} // end pushProjectile method

PixNode *PixMap::getPixNode(Vect pos)
{
    return pixNodes [(int)pos.x] [(int)pos.y];
} // end getPixNode

int PixMap::getSlopeAngle(Vect pos)
{
    int averageRadius = 3;
    int numAveraged = averageRadius * 2 + 1;
    int heights [numAveraged];
    float average = 0;

    for (int i = pos.x - averageRadius; i <= pos.x + averageRadius; i += 1)
    {
        int tempY = pos.y;
        if (i < 0 || i > mapWidth - 1) {
            tempY = pos.y;
        } else {
            if (pixNodes [i] [tempY] != NULL && !checkPixLiquid(Vect(i, tempY))) {
                while (pixNodes [i] [tempY - 1] != NULL && !checkPixLiquid(Vect(i, tempY - 1)) && tempY > 0)
                {
                    tempY -= 1;
                } // end while
            } else {
                if (tempY < mapHeight - 1) {
                    do
                    {
                        tempY += 1;
                    } while (pixNodes [i] [tempY] == NULL && !checkPixLiquid(Vect(i, tempY)) && tempY < mapHeight - 1);
                }
            }
        }
        heights [i - ((int)pos.x - averageRadius)] = tempY;
    } // end for (i)
    for (int i = 0; i < numAveraged; i += 1)
        average += heights [i];
    average /= numAveraged;
    return radtoDegree(atan((heights[0] - average) / (numAveraged / 2)));
} // end getSlope method

void PixMap::genPixMap(mapType type)
{
    int maxSegLength;
    float smoothness;
    float slopeMax;
    float startY;
    if (type == SANDSEA) {
        maxSegLength = 7;
        smoothness = 2;
        slopeMax = 5;
        startY = mapHeight - 10;
    }
    for (int i = 0; i < mapWidth; i += 1)
        for (int j = 0; j < mapHeight; j += 1) {
            deletePixNode({i, j});
        } // end for (j)

    int seed = time(NULL); // Generate a psuedo random seed
    srand(seed); // set the random seed

    int segLength = rand() % maxSegLength + 1;

    float slope = 0, prevSlope = 0;

    float colR, colG, colB, colA; /** Temporary **/

    for (int i = 0; i < mapWidth; i += 1) {
        if (slope <= 0) {
            colR = 0.45f; /** Temporary **/
            colG = 0.25f; /** Temporary **/
            colB = 0.65f; /** Temporary **/
            colA = 1.0f; /** Temporary **/
        } else {
            colR = 0.5f; /** Temporary possible - slope / slopeMax / 8 **/
            colG = 0.3f; /** Temporary possible - slope / slopeMax / 8 **/
            colB = 0.7f; /** Temporary possible - slope / slopeMax / 8 **/
            colA = 1.0f; /** Temporary possible - slope / slopeMax / 8 **/
        }
        for (int j = startY; j < mapHeight; j += 1) {
            if ((int)(j - startY) % 8 == 0) { /** Also (j + i) % 7 for diagonal lines, j % 7 for blocks **/
                colR -= 0.02;  /** Temporary **/
                colG -= 0.02;  /** Temporary **/
                colB -= 0.02;  /** Temporary **/
            }
            createPixNode({i, j}, SAND, {colR, colG, colB, colA});
        } // end for (j)

        if (i % segLength == 0) {
            if (startY > mapHeight / 4 && startY < mapHeight - 1) {
                do {
                    slope = (float)rand() / (float)RAND_MAX * (slopeMax * 2 + 1) - slopeMax; // generate new slope
                } while (fabs(prevSlope - slope) > smoothness); // prevents generation of spiky PixMap
            } else if (startY >= mapHeight - 1){
                slope = (float)rand() / (float)RAND_MAX * (slopeMax * 2 + 1) - slopeMax * 1.25;
            } else {
                slope = (float)rand() / (float)RAND_MAX * (slopeMax * 2 + 1) - slopeMax * 0.75;
            }
            prevSlope = slope;
            segLength = rand() % (maxSegLength - 2) + 2; // generate new seglength
        }
        startY += slope / segLength;

        // make sure the start position is not oustide the bounds of the screen
        if (startY < 0) {
            startY = 0;
        } else if (startY > mapHeight) {
            startY = mapHeight;
        }
    } // end for (i)
} // end genPixMap method

void PixMap::addActivePlayers(std::vector <Player*> *activePlayers)
{
    this->activePlayers = activePlayers;
} // end addActivePlayers method

void PixMap::tripActives(Vect pos) /** give function to pixMap perhaps **/
{
    if(pos.y > 0 && checkPixCollide({pos.x, pos.y - 1})) {
        pushActivePixNode({pos.x, pos.y - 1});
    } else if((pos.x < mapWidth - 1 && checkPixCollide({pos.x + 1, pos.y})) && (pos.x > 0 && checkPixCollide({pos.x - 1, pos.y}))) {
        if (rand() % 2 == 0) {
            pushActivePixNode({pos.x + 1, pos.y});
        } else {
            pushActivePixNode({pos.x - 1, pos.y});
        }
    } else if (pos.x < mapWidth - 1 && checkPixCollide({pos.x + 1, pos.y})) {
        pushActivePixNode({pos.x + 1, pos.y});
    } else if(pos.x > 0 && checkPixCollide({pos.x - 1, pos.y})) {
        pushActivePixNode({pos.x - 1, pos.y});
    }
} // end tripActives method

void PixMap::activateProjectiles()
{
    std::vector<int> removeIndices;
    int offset = 0;
    for (std::vector<Projectile*>::iterator i = activeProjectiles.begin(); i < activeProjectiles.end(); i += 1) {
        if((*i)->onActivate()) {
            removeIndices.push_back(i - activeProjectiles.begin());
            /** create deleter **/
        }
    } // end for (i)
    for (std::vector<int>::iterator i = removeIndices.begin(); i < removeIndices.end(); i += 1) {
        activeProjectiles.erase(activeProjectiles.begin() + (*i) - offset);
        offset += 1;
    } // end for (i)
} // end activeProjectiles method

bool PixMap::checkPixCollide(Vect pos)
{
    if (pos.x < 0 || pos.x > mapWidth - 1 || pos.y > mapHeight - 1) {
        return true;
    } else if (pos.y < 0) {
        return false;
    }
    if (pixNodes[(int)pos.x][(int)pos.y] != NULL) {
        return true;
    }
    return false;
} // end checkPixNode method

bool PixMap::checkPixActive(Vect pos)
{
    if (pos.x < 0 || pos.x > mapWidth - 1 || pos.y < 0 || pos.y > mapHeight - 1)
        return false;
    if (getPixNode(pos) != NULL) {
        return getPixNode(pos)->checkActive();
    }
    return false;
} // end checkPixNode method

bool PixMap::checkPixLiquid(Vect pos)
{
    if (pos.x < 0 || pos.x > mapWidth - 1 || pos.y < 0 || pos.y > mapHeight - 1)
        return false;
    if (getPixNode(pos) != NULL) {
        return getPixNode(pos)->checkLiquid();
    }
} // end checkPixLiquid method

bool PixMap::checkPixType(Vect pos, int type)
{
    if (pos.x < 0 || pos.x > mapWidth - 1 || pos.y < 0 || pos.y > mapHeight - 1)
        return false;
    if (getPixNode(pos) != NULL) {
        return (getPixNode(pos)->getType() == type);
    }
    return false;
} // end checkPixNode method

bool PixMap::checkInBounds(Vect pos)
{
    if (pos.x < 0 || pos.x > mapWidth - 1 || pos.y > mapHeight - 1)
        return true;
    return false;
} // end checkInBounds method

bool PixMap::checkPlayerCollide(Vect pos)
{
    for(std::vector<Player*>::iterator i = (*activePlayers).begin(); i < (*activePlayers).end(); i += 1)
    {
        if ((*i)->checkCollision(pos))
            return true;
    } // end for (i)
    return false;
} // end checkPixNode method

void PixMap::checkPlayerDamages(Vect pos, float damage, float radius) /** Needs to be made to work with radius **/
{
    for(std::vector<Player*>::iterator i = (*activePlayers).begin(); i < (*activePlayers).end(); i += 1)
    {
        if ((*i)->checkCollision(pos) || (*i)->checkRadialCollision(pos, radius))
            (*i)->damage(damage);
    } // end for (i)
} // end checkPlayerDamages method

void PixMap::swapNodes(Vect pos1, Vect pos2)
{
    PixNode *tempNode = getPixNode(pos1);
    pixNodes[(int)pos1.x][(int)pos1.y] = getPixNode(pos2);
    pixNodes[(int)pos2.x][(int)pos2.y] = tempNode;

    if (getPixNode(pos1) != NULL) {
        getPixNode(pos1)->setPos(pos1); /** May be wrong **/
    } else {
        tripActives(pos1);
    }
    if (getPixNode(pos2) != NULL) {
        getPixNode(pos2)->setPos(pos2); /** May be wrong **/
    } else {
        tripActives(pos2);
    }
} // end swapNodes method

void PixMap::draw()
{
    for (int i = 0; i < mapWidth; i += 1) {
        for (int j = 0; j < mapHeight; j += 1) {
            if (pixNodes[i] [j] != NULL) {
                pixNodes [i] [j]->draw();
            }
        } // end for (j)
    } // end for (i)
    drawEditSections();
    drawProjectiles();
} // end draw method

void PixMap::step()
{
    Vect pos(rand() % mapWidth, 0);
    createPixNode(pos, WATER, {0.4f, 0.3f, 0.7f, 0.6f});

    unloadEditSectionStack();
    stepEditSections();

    unloadProjectileStack();
    stepProjectiles();

    unloadPixNodeStack();
    stepPixNodes();
} // end step method

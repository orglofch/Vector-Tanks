/**
*
* @author: Owen Glofcheski
* @date: January 31, 2012
*
*/

#ifndef PIXMAP_H_INCLUDED
#define PIXMAP_H_INCLUDED

#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include "Globals.h"
#include "Effects.h"

class PixEdit;
class PixNode;
class Player;
class Projectile;

enum mapType {
    SANDSEA = 0
};

class PixMap
{
private:
    PixNode ***pixNodes;

    std::vector <PixEdit*> activeEditSections;
    std::vector <PixEdit*> stackEditSections;

    std::vector <Projectile*> activeProjectiles;
    std::vector <Projectile*> stackProjectiles; // Stack's all calls to create new Projectiles and
                                                // Unloads them at the beginning of each step
    std::vector <Player*> *activePlayers;

    std::vector <PixNode*> *activePixNodes;
    std::vector <PixNode*> stackPixNodes;    // Stack's all calls to create new PixNode and
                                                // Unloads them at the beginning of each step

    void drawEditSections();
    void drawProjectiles();

    void stepEditSections();
    void stepProjectiles();
    void stepPixNodes();

    void unloadEditSectionStack();
    void unloadProjectileStack();
    void unloadPixNodeStack();
public:
    PixMap();
    ~PixMap();

    void deletePixNode(Vect pos);
    void createPixNode(Vect pos, int type, Color color);

    // Remove, Collapse, Build method - Circle
    void pushRemove(Vect pos, int radius);
    void pushCollapse(Vect pos, int radius);
    void pushBuild(Vect pos, int height, int width);
    void pushLiquify(Vect pos, int radius);

    void pushActivePixNode(Vect pos);

    // Projectile Pushes
    void pushProjectile(Projectile *newProjectile);

    PixNode *getPixNode(Vect pos);
    int getSlopeAngle(Vect pos);

    void loadTerrain(std::string file);
    void genPixMap(mapType type);
    void addActivePlayers(std::vector <Player*> *activePlayers);
    void tripActives(Vect pos);
    void activateProjectiles();

    /** Create a pixNode color burning and add it to Burrower **/

    bool checkPixCollide(Vect pos); /** rename to something better **/
    bool checkPixActive(Vect pos);
    bool checkPixType(Vect pos, int type);
    bool checkPixLiquid(Vect pos);
    bool checkInBounds(Vect pos); /** Not implemented yet **/
    bool checkPlayerCollide(Vect pos);
    void checkPlayerDamages(Vect pos, float damage, float radius);

    void swapNodes(Vect pos1, Vect pos2);

    void draw();
    void step();
};

#endif // PIXMAP_H_INCLUDED

/**
*
* @author: Owen Glofcheski
* @date: February 27, 2012
*
*/

#ifndef TANK_H_INCLUDED
#define TANK_H_INCLUDED

#include <map>
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>

#include <gl/gl.h>
#include "glfont.h"

#include "Weapons.h"
#include "PixMap.h"
#include "Effects.h"
#include "Color.h"
#include "Vect.h"
#include "Math.h"
#include "CollisionBox.h"
#include "UIEntity.h"

class Player
{
protected:
    PixMap *pixMap;
    Timer *turnTimer;

    int playerNum;

    float gravity;
    float drag;

    // Attributes and UI
    Container *UI;

    float energy;
    PercentageBar *energyBar;
    float energyWell;

    float health;
    float accumDamage;
    float damageWell;
    PercentageBar *healthBar;

    float fuel;
    PercentageBar *fuelBar;

    float power;
    int currency; /** Rename to something else **/
    float deathDamage;
    float deathRadius;

    bool isDead;
    bool isPaused;

    int dieCounter;

    Color color;
    float size;

    int currentProjIndex;
    std::vector<std::pair<projectileType, int> > availableProj;
    Container *weapons;

    float cannonAngle;
    float shellAngle;

    Vect pos;
    Vect velo;
    Vect joint;
    Vect cannon;
    Vect Reticule; /** Implement **/
    std::vector<Vect> jets;
    CollisionBox collisionBox;
    std::vector<Vect> moveForces;

    float cannonLength;

    int stepCounter;
    int fireCounter;
    float shakeAngle;

    float iconBob;
    float iconBobInc;

    // private methods
    Vect rayCollide();

    virtual void drawCannon() = 0;
    virtual void drawShell() = 0;
    void drawReticule();

    virtual void setRelationPoints() = 0;

    // STEP METHODS
    void stepMovement(); /** Not implemented **/
    void stepDamage();
    void stepEnergy();
    void fireStep();
    bool dieStep();
    void aliveStep();

    // UI METHODS
    void UIInit();
    Container *createHeader(UIEntity *mainMenu, UIEntity *audioMenu);
    Container *createFooter(UIEntity *shop);
    Container *createShop();
    void setWeaponPurchaseUI(Container *weaponPreviewWindow, Container *terraformingList, Container *destructiveList, Container *utilityList, Button *listMode,Button *iconMode);
    void setShipPurchaseUI(Container *shipPreviewWindow, Container *movementList, Container *defenceList, Container *offenceList, Container *utilityList, Button *listMode, Button *iconMode);

    void addMovements();
    virtual void createJetParticles(Vect force) = 0;
public:
    Player(PixMap *pixMap, int playerNum);
    ~Player();

    void setAngle(float angle);
    void setPower(float power);
    void setPosition(Vect pos);
    void setColor(Color color);

    float getAngle();
    Vect getPosition();

    bool checkCollision(Vect pos);
    bool checkRadialCollision(Vect pos, float radius);

    void addWeapon();
    void nextProjectile(); /** change Projectile to weapon **/
    void previousProjectile();
    void fireProjectile();
    void damage(float damage);

    void stepIcon();
    void drawIcon();

    void click(Vect mPos);
    void release(Vect mPos);
    void stepUI(Vect mPos);
    void drawUI();
    bool checkTurnTimer();
    void resetTurnTimer();

    void draw();
    bool step();

    void move(Vect force);
    void addForce(Vect force);
    void kill();
};

class Tank : public Player
{
private:
    float cannonLength;

    void drawCannon();
    void drawShell();

    void setRelationPoints();
    void createJetParticles(Vect force);
public:
    Tank(PixMap *pixMap, int playerNum);
};

class SpaceInvader : public Player
{
private:
    void drawCannon();
    void drawShell();

    void setRelationPoints();
    void createJetParticles(Vect force);
public:
    SpaceInvader(PixMap *pixMap, int playerNum);
};

class GeometryShip : public Player
{
private:
    void drawCannon();
    void drawShell();

    void setRelationPoints();
    void createJetParticles(Vect force);
public:
    GeometryShip(PixMap *pixMap, int playerNum);
};

class Motherload : public Player
{
private:
    void drawCannon();
    void drawShell();

    void setRelationPoints();
    void createJetParticles(Vect force);
public:
    Motherload(PixMap *pixMap, int playerNum);
};

#endif // TANK_H_INCLUDED

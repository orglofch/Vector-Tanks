/**
*
* @author: Owen Glofcheski
* @date: March 19, 2012
*
*/

#ifndef WEAPONS_H_INCLUDED
#define WEAPONS_H_INCLUDED

/** Make not absolute pathing **/
#include "Weapons/Projectile.h"
#include "Weapons/Burrower.h"
#include "Weapons/Nebula.h"
#include "Weapons/Splitter.h"
#include "Weapons/Torrent.h"
#include "Weapons/Undermine.h"
#include "Weapons/SharpShot.h"
#include "Weapons/EMP.h"
#include "Weapons/Pyroclasm.h"
#include "Weapons/ClusterBomb.h"
#include "Weapons/RicochetRifle.h"
#include "Weapons/Fireworks.h"
#include "Weapons/BlackSheepWall.h"
#include "Weapons/Skipper.h"
#include "Weapons/Epicenter.h"

enum projectileType {
    NORMAL = 0, /** Rename **/
    SPLITTER = 1,
    NEBULA = 2,
    BURROWER = 3,
    TORRENT = 4,
    UNDERMINE = 5,
    SHARPSHOT = 6,
    EMP = 7,
    PYROCLASM = 8,
    CLUSTERBOMB = 9,
    RICOCHETRIFLE = 10,
    BLACKSHEEPWALL = 11,
    FIREWORKS = 12,
    SKIPPER = 13,
    EPICENTER = 14,

    COUNT = 15
};

#endif // WEAPONS_H_INCLUDED

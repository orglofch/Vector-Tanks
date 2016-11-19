/**
*
* @author: Owen Glofcheski
* @date: April 28th, 2012
*
*/

#ifndef WEAPON_DATA_H_INCLUDED
#define WEAPON_DATA_H_INCLUDED

#include <string>

struct LevelData {
    int buyCost;
    float damage;
    float radius;
    int numProjectile;
    int bounceCount;
    int energyCost;
};

struct WeaponData {
    std::string name;
    std::string upgradeName;

    LevelData levels [3];

    std::string imgURL;

    WeaponData(std::string name, std::string upgradeName, LevelData levels [3], std::string imgURL);
};

#endif // WEAPON_DATA_H_INCLUDED

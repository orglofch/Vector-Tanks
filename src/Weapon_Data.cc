#include "Weapon_Data.h"

WeaponData::WeaponData(std::string name, std::string upgradeName, LevelData levels [3], std::string imgURL)
{
    this->name = name;
    this->upgradeName = upgradeName;
    this->levels[0] = levels[0];
    this->levels[1] = levels[1];
    this->levels[2] = levels[2];

    this->imgURL = "Media/" + imgURL;
} // end constructor

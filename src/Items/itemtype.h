#ifndef ITEMTYPE_H
#define ITEMTYPE_H

#include <SFML/Graphics/Rect.hpp>
#include <vector>

class HitBox;

enum ItemType
{
    ITEM_LIFE,
    ITEM_EXP1,
    ITEM_EXP5,
    ITEM_EXP20,
    ITEM_EXP100,
    ITEM_EXP500,
    ITEM_EXP2K,
    ITEM_EXP10K
};

bool isExperienceType(ItemType type);
unsigned int experienceValue(ItemType type);
std::vector<ItemType> experienceToItems(unsigned int value);
sf::FloatRect itemTextureRect(ItemType type);
float itemPickRadius(ItemType type);
HitBox itemBox(ItemType type);
const float itemDropSpeed(5.0f);

#endif // ITEMTYPE_H

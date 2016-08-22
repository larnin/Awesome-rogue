#include "itemtype.h"
#include "Collisions/hitbox.h"
#include "Map/blocktype.h"
#include <cassert>

bool isExperienceType(ItemType type)
{
    switch(type)
    {
    case ITEM_EXP1:
    case ITEM_EXP5:
    case ITEM_EXP20:
    case ITEM_EXP100:
    case ITEM_EXP500:
    case ITEM_EXP2K:
    case ITEM_EXP10K:
        return true;
    default:
        return false;
    }
}

unsigned int experienceValue(ItemType type)
{
    assert(isExperienceType(type));

    switch (type)
    {
    case ITEM_EXP1:
        return 1;
    case ITEM_EXP5:
        return 5;
    case ITEM_EXP20:
        return 20;
    case ITEM_EXP100:
        return 100;
    case ITEM_EXP500:
        return 500;
    case ITEM_EXP2K:
        return 2000;
    case ITEM_EXP10K:
        return 10000;
        return true;
    default:
        return 1;
    }
}

std::vector<ItemType> experienceToItems(unsigned int value)
{
    std::vector<ItemType> items;

    const std::vector<ItemType> types{ITEM_EXP10K, ITEM_EXP2K, ITEM_EXP500, ITEM_EXP100, ITEM_EXP20, ITEM_EXP5, ITEM_EXP1};

    for(ItemType type : types)
    {
        for(unsigned int i(0) ; i < value/experienceValue(type) ; i++)
            items.push_back(type);
        value %= experienceValue(type);
    }

    return items;
}

sf::FloatRect itemTextureRect(ItemType type)
{
    switch(type)
    {
    case ITEM_EXP1:
        return sf::FloatRect(12, 0, 7, 7);
    case ITEM_EXP5:
        return sf::FloatRect(20, 0, 7, 7);
    case ITEM_EXP20:
        return sf::FloatRect(28, 0, 8, 8);
    case ITEM_EXP100:
        return sf::FloatRect(37, 0, 9, 9);
    case ITEM_EXP500:
        return sf::FloatRect(47, 0, 11, 11);
    case ITEM_EXP2K:
        return sf::FloatRect(59, 0, 17, 17);
    case ITEM_EXP10K:
        return sf::FloatRect(77, 0, 13, 21);
    case ITEM_LIFE:
        return sf::FloatRect(0, 0, 11, 11);
    default:
        return sf::FloatRect();
    }
}

float itemPickRadius(ItemType type)
{
    switch(type)
    {
    case ITEM_EXP1:
    case ITEM_EXP5:
    case ITEM_EXP20:
    case ITEM_EXP100:
    case ITEM_EXP500:
        return 0.1f;
    case ITEM_EXP2K:
    case ITEM_EXP10K:
        return 0.2f;
    case ITEM_LIFE:
        return 0.5f;
    default:
        return 1.0f;
    }
}

HitBox itemBox(ItemType type)
{
    HitBox b;

    sf::FloatRect rect(itemTextureRect(type));
    b.addLine(Line(sf::Vector2f(-rect.width/2, -rect.height/2), sf::Vector2f(-rect.width/2, rect.height/2)));
    b.addLine(Line(sf::Vector2f(-rect.width/2, rect.height/2), sf::Vector2f(rect.width/2, rect.height/2)));
    b.addLine(Line(sf::Vector2f(rect.width/2, rect.height/2), sf::Vector2f(rect.width/2, -rect.height/2)));
    b.addLine(Line(sf::Vector2f(rect.width/2, -rect.height/2), sf::Vector2f(-rect.width/2, -rect.height/2)));

    return b.transform(1.0f/BlockType::tileSize);
}

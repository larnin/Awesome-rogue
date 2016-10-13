#include "blocktype.h"
#include "block.h"
#include "Collisions/hitbox.h"
#include "blockinteractiontype.h"
#include "Utilities/tr.h"

const float PI(3.14159f);

const unsigned int BlockType::tileSize(16);
const unsigned int BlockType::nbTile(32);

bool BlockType::isGround(unsigned int id)
{
    if(id >= 1 && id <=3)
        return false;
    return true;
}

bool BlockType::isWall(unsigned int id)
{
    return !isGround(id);
}

BoxType BlockType::boxOf(unsigned int id)
{
    if(isWall(id))
        return BoxType::FULL;
    return BoxType::EMPTY;
}

HitBox BlockType::createBox(unsigned char boxCaracts)
{
    BoxType type(getBoxType(boxCaracts));
    HitBox box;
    switch (type)
    {
    case BoxType::FULL:
        box.addLine(Line(sf::Vector2f(-0.5f, -0.5f), sf::Vector2f(-0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(-0.5f, 0.5f), sf::Vector2f(0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(0.5f, -0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, -0.5f), sf::Vector2f(-0.5f, -0.5f)));
        break;
    case BoxType::TRIANGLE:
        box.addLine(Line(sf::Vector2f(-0.5f, 0.5f), sf::Vector2f(0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(0.5f, -0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, -0.5f), sf::Vector2f(-0.5f, 0.5f)));
        break;
    case BoxType::HALF:
        box.addLine(Line(sf::Vector2f(-0.5f, 0.0f), sf::Vector2f(-0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(-0.5f, 0.5f), sf::Vector2f(0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(0.5f, 0.0f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.0f), sf::Vector2f(-0.5f, 0.0f)));
        break;
    case BoxType::QUARTER:
        box.addLine(Line(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.0f, 0.5f), sf::Vector2f(0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(0.5f, 0.0f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.0f), sf::Vector2f(0.0f, 0.0f)));
        break;
    case BoxType::THREE_QUARTER:
        box.addLine(Line(sf::Vector2f(-0.5f, -0.5f), sf::Vector2f(0.5f, -0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, -0.5f), sf::Vector2f(0.5f, 0.0f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.0f), sf::Vector2f(0.0f, 0.0f)));
        box.addLine(Line(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.0f, 0.5f), sf::Vector2f(-0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(-0.5f, 0.5f), sf::Vector2f(-0.5f, -0.5f)));
        break;
    case BoxType::HALF_CENTRED:
        box.addLine(Line(sf::Vector2f(-0.5f, -0.25f), sf::Vector2f(-0.5f, 0.25f)));
        box.addLine(Line(sf::Vector2f(-0.5f, 0.25f), sf::Vector2f(0.5f, 0.25f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.25f), sf::Vector2f(0.5f, -0.25f)));
        box.addLine(Line(sf::Vector2f(0.5f, -0.25f), sf::Vector2f(-0.5f, -0.25f)));
        break;
    case BoxType::CROSS:
        box.addLine(Line(sf::Vector2f(-0.5f, -0.25f), sf::Vector2f(-0.25f, -0.25f)));
        box.addLine(Line(sf::Vector2f(-0.25f, -0.25f), sf::Vector2f(-0.25f, -0.5f)));
        box.addLine(Line(sf::Vector2f(-0.25f, -0.5f), sf::Vector2f(0.25f, -0.5f)));
        box.addLine(Line(sf::Vector2f(0.25f, -0.5f), sf::Vector2f(0.25f, -0.25f)));
        box.addLine(Line(sf::Vector2f(0.25f, -0.25f), sf::Vector2f(0.5f, -0.25f)));
        box.addLine(Line(sf::Vector2f(0.5f, -0.25f), sf::Vector2f(0.5f, 0.25f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.25f), sf::Vector2f(0.25f, 0.25f)));
        box.addLine(Line(sf::Vector2f(0.25f, 0.25f), sf::Vector2f(0.25f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.25f, 0.5f), sf::Vector2f(-0.25f, 0.5f)));
        box.addLine(Line(sf::Vector2f(-0.25f, 0.5f), sf::Vector2f(-0.25f, 0.25f)));
        box.addLine(Line(sf::Vector2f(-0.25f, 0.25f), sf::Vector2f(-0.5f, 0.25f)));
        box.addLine(Line(sf::Vector2f(-0.5f, 0.25f), sf::Vector2f(-0.5f, -0.25f)));
        break;
    case BoxType::T_SHARP:
        box.addLine(Line(sf::Vector2f(-0.5f, -0.25f), sf::Vector2f(0.5f, -0.25f)));
        box.addLine(Line(sf::Vector2f(0.5f, -0.25f), sf::Vector2f(0.5f, 0.25f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.25f), sf::Vector2f(0.25f, 0.25f)));
        box.addLine(Line(sf::Vector2f(0.25f, 0.25f), sf::Vector2f(0.25f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.25f, 0.5f), sf::Vector2f(-0.25f, 0.5f)));
        box.addLine(Line(sf::Vector2f(-0.25f, 0.5f), sf::Vector2f(-0.25f, 0.25f)));
        box.addLine(Line(sf::Vector2f(-0.25f, 0.25f), sf::Vector2f(-0.5f, 0.25f)));
        box.addLine(Line(sf::Vector2f(-0.5f, 0.25f), sf::Vector2f(-0.5f, -0.25f)));
        break;
    case BoxType::CORNER_CENTRED:
        box.addLine(Line(sf::Vector2f(-0.25f, -0.25f), sf::Vector2f(0.5f, -0.25f)));
        box.addLine(Line(sf::Vector2f(0.5f, -0.25f), sf::Vector2f(0.5f, 0.25f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.25f), sf::Vector2f(0.25f, 0.25f)));
        box.addLine(Line(sf::Vector2f(0.25f, 0.25f), sf::Vector2f(0.25f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.25f, 0.5f), sf::Vector2f(-0.25f, 0.5f)));
        box.addLine(Line(sf::Vector2f(-0.25f, 0.5f), sf::Vector2f(-0.25f, -0.25f)));
        break;
    default:
        break;
    }

    float rot(0.0f);
    switch(getRotation(boxCaracts))
    {
    case Rotation::ROT_90:
        rot = PI/2.0f;
        break;
    case Rotation::ROT_180:
        rot = PI;
        break;
    case Rotation::ROT_270:
        rot = 3*PI/2.0f;
        break;
    default:
        break;
    }
    return box.transform(rot, getXFlip(boxCaracts), getYFlip(boxCaracts));
}

BlockInteractionType BlockType::getInteraction(const Block & b)
{
    BlockInteractionType bi(getInteraction(b.groundID));
    if(bi != BI_NO_INTERACTION)
        return bi;
    return getInteraction(b.wallID);
}

BlockInteractionType BlockType::getInteraction(unsigned int id)
{
    const unsigned int chestID(6);
    const unsigned int portalID(8);
    const unsigned int saveID(9);
    const unsigned int bossID1(513);

    switch(id)
    {
    case chestID:
        return BI_CHEST;
    case portalID:
        return BI_PORTAL;
    case saveID:
        return BI_SAVEPOINT;
    case bossID1:
        return BI_START_BOSS1;
    default:
        return BI_NO_INTERACTION;
    }
}

std::string BlockType::interactionName(BlockInteractionType b)
{
    switch(b)
    {
    case BI_CHEST:
        return tr("Open");
    case BI_PORTAL:
        return tr("Teleport");
    case BI_SAVEPOINT:
        return tr("Save");
    case BI_START_BOSS1:
        return tr("Activate");
    default:
        return "";
    }
}

#include "blocktype.h"

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
        box.addLine(Line(sf::Vector2f(-0.5f, -0.0f), sf::Vector2f(0.0f, 0.0f)));
        box.addLine(Line(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, -0.5f)));
        box.addLine(Line(sf::Vector2f(0.0f, -0.5f), sf::Vector2f(-0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(-0.5f, 0.5f), sf::Vector2f(0.5f, 0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, 0.5f), sf::Vector2f(0.5f, -0.5f)));
        box.addLine(Line(sf::Vector2f(0.5f, -0.5f), sf::Vector2f(-0.5f, -0.0f)));
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

#ifndef ORIENTATION_H
#define ORIENTATION_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

enum Orientation
{
    UP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3,
    UNORIENTED = -1
};

template <typename T>
sf::Vector2<T> orientationToVector(Orientation o)
{
    switch(o)
    {
    case Orientation::UP:
        return sf::Vector2<T>(0, -1);
    case Orientation::LEFT:
        return sf::Vector2<T>(-1, 0);
    case Orientation::DOWN:
        return sf::Vector2<T>(0, 1);
    case Orientation::RIGHT:
        return sf::Vector2<T>(1, 0);
    default:
        return sf::Vector2<T>(0, 0);
    }
    return sf::Vector2<T>(0, 0);
}

template <typename T>
sf::Vector2<T> relative(sf::Vector2<T> pos, Orientation o, T distance)
{
    return pos + orientationToVector<T>(o)*distance;
}

template <typename T>
sf::Vector2<T> relative(sf::Vector2<T> pos, Orientation o)
{
    return pos + orientationToVector<T>(o);
}

template <typename T>
sf::Vector2<T> relativeOn(sf::Vector2<T> pos, Orientation o, sf::Rect<T> rect)
{
    switch(o)
    {
    case Orientation::UP:
        return sf::Vector2<T>(pos.x, rect.top);
    case Orientation::LEFT:
        return sf::Vector2<T>(rect.left, pos.y);
    case Orientation::DOWN:
        return sf::Vector2<T>(pos.x, rect.top+rect.height);
    case Orientation::RIGHT:
        return sf::Vector2<T>(rect.left+rect.width, pos.y);
    default:
        return pos;
    }
    return pos;
}

template <typename T>
Orientation orientedSide(sf::Vector2<T> pos, sf::Rect<T> rect)
{
    if(pos.x <= rect.left)
        return Orientation::LEFT;
    if(pos.x >= rect.left+rect.width-1)
        return Orientation::RIGHT;
    if(pos.y <= rect.top)
        return Orientation::UP;
    if(pos.y >= rect.top+rect.height-1)
        return Orientation::DOWN;
    return Orientation::UNORIENTED;
}

#endif // ORIENTATION_H

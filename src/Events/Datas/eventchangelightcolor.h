#ifndef EVENTCHANGELIGHTCOLOR_H
#define EVENTCHANGELIGHTCOLOR_H

#include <SFML/Graphics/Color.hpp>

struct EventChangeLightColor
{
    EventChangeLightColor(const sf::Color & _base, const sf::Color & _saturated)
        : base(_base)
        , saturated(_saturated)
    {

    }

    sf::Color base;
    sf::Color saturated;
};

#endif // EVENTCHANGELIGHTCOLOR_H

#ifndef EVENTSETAMBIANTCOLOR_H
#define EVENTSETAMBIANTCOLOR_H

#include <SFML/Graphics/Color.hpp>

struct EventSetAmbiantColor
{
    EventSetAmbiantColor(const sf::Color & c)
        : color(c)
    {

    }

    sf::Color color;
};

#endif // EVENTSETAMBIANTCOLOR_H

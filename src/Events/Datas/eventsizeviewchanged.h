#ifndef EVENTSIZEVIEWCHANGED_H
#define EVENTSIZEVIEWCHANGED_H

#include <SFML/System/Vector2.hpp>

struct EventSizeViewChanged
{
    EventSizeViewChanged(const sf::Vector2f & newSize)
        : size(newSize)
    { }

    sf::Vector2f size;
};

#endif // EVENTSIZEVIEWCHANGED_H

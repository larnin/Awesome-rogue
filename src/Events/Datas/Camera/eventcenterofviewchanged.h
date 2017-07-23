#ifndef EVENTCENTEROFVIEWCHANGED_H
#define EVENTCENTEROFVIEWCHANGED_H

#include <SFML/System/Vector2.hpp>

struct EventCenterOfViewChanged
{
    EventCenterOfViewChanged(const sf::Vector2f & _pos)
        : pos(_pos)
    {

    }

    sf::Vector2f pos;
};

#endif // EVENTCENTEROFVIEWCHANGED_H

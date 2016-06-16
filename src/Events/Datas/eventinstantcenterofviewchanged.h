#ifndef EVENTINSTANTCENTEROFVIEWCHANGED_H
#define EVENTINSTANTCENTEROFVIEWCHANGED_H

#include <SFML/System/Vector2.hpp>

struct EventInstantCenterOfViewChanged
{
    EventInstantCenterOfViewChanged(const sf::Vector2f & _pos)
        : pos(_pos)
    {

    }

    sf::Vector2f pos;
};

#endif // EVENTINSTANTCENTEROFVIEWCHANGED_H

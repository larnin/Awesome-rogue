#ifndef EVENTDELLIGHT_H
#define EVENTDELLIGHT_H

#include <memory>

template <typename T>
struct EventDelLight
{
    EventDelLight(const std::weak_ptr<T> & l)
        : light(l)
    {

    }

    std::weak_ptr<T> light;
};

#include "Lights/Types/pointlight.h"
using EventDelPointLight = EventDelLight<PointLight>;

#endif // EVENTDELLIGHT_H

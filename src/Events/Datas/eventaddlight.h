#ifndef EVENTADDLIGHT_H
#define EVENTADDLIGHT_H

#include <memory>

namespace
{
template <typename T>
struct EventAddLight
{
    EventAddLight(const std::weak_ptr<T> & l)
        : light(l)
    {

    }

    std::weak_ptr<T> light;
};
}

#include "Lights/Types/pointlight.h"
using EventAddPointLight = EventAddLight<PointLight>;

#endif // EVENTADDLIGHT_H

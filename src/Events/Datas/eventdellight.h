#ifndef EVENTDELLIGHT_H
#define EVENTDELLIGHT_H

#include <memory>
#include "Lights/Types/light.h"

struct EventDelLight
{
    EventDelLight(const std::weak_ptr<Light> & l)
        : light(l)
    {

    }

    std::weak_ptr<Light> light;
};

#endif // EVENTDELLIGHT_H

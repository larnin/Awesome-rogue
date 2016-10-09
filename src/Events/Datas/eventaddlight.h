#ifndef EVENTADDLIGHT_H
#define EVENTADDLIGHT_H

#include "Lights/Types/light.h"
#include <memory>

struct EventAddLight
{
    EventAddLight(const std::weak_ptr<Light> & l)
        : light(l)
    {

    }

    std::weak_ptr<Light> light;
};

#endif // EVENTADDLIGHT_H

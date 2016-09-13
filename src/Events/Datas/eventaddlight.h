#ifndef EVENTADDLIGHT_H
#define EVENTADDLIGHT_H

#include <memory>
#include "Lights/Types/light.h"

struct EventAddLight
{
    EventAddLight(const std::weak_ptr<Light> & l)
        : light(l)
    {

    }

    std::weak_ptr<Light> light;
};

#endif // EVENTADDLIGHT_H

#ifndef EVENTADDLIGHT_H
#define EVENTADDLIGHT_H

#include "Lights/lightdata.h"
#include <memory>

struct EventAddLight
{
    EventAddLight(const std::weak_ptr<LightData> & l)
        : light(l)
    {

    }

    std::weak_ptr<LightData> light;
};

#endif // EVENTADDLIGHT_H

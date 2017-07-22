#ifndef EVENTDELLIGHT_H
#define EVENTDELLIGHT_H

#include "Lights/lightdata.h"
#include <memory>

template <typename T>
struct EventDelLight
{
    EventDelLight(const std::weak_ptr<LightData> & l)
        : light(l)
    {

    }

    std::weak_ptr<LightData> light;
};

#endif // EVENTDELLIGHT_H

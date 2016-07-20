#ifndef EVENTPLAYCAMERAEFFECT_H
#define EVENTPLAYCAMERAEFFECT_H

#include "Machine/CameraEffects/cameraeffecttype.h"

struct EventPlayCameraEffect
{
    EventPlayCameraEffect(CameraEffectType _type, float _time)
        : type(_type)
        , time(_time)
    {

    }

    CameraEffectType type;
    float time;
};

#endif // EVENTPLAYCAMERAEFFECT_H

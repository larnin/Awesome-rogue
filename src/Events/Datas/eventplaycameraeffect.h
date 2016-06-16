#ifndef EVENTPLAYCAMERAEFFECT_H
#define EVENTPLAYCAMERAEFFECT_H

enum CameraEffectType
{

};

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
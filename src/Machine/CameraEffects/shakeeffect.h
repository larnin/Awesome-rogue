#ifndef SHAKEEFFECT_H
#define SHAKEEFFECT_H

#include <random>
#include "cameraeffect.h"

class ShakeEffect : public CameraEffect
{
public:
    ShakeEffect(float maxValue);
    ShakeEffect(ShakeEffect &&) = default;
    ShakeEffect & operator =(ShakeEffect &&) = default;
    virtual ~ShakeEffect() = default;

    virtual void update(const sf::Time & elapsedTime);

private:
    float m_maxValue;
    std::default_random_engine m_rand;
};

#endif // SHAKEEFFECT_H

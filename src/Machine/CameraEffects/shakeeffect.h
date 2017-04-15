#ifndef SHAKEEFFECT_H
#define SHAKEEFFECT_H

#include "cameraeffect.h"
#include <random>

class ShakeEffect : public CameraEffect
{
public:
    ShakeEffect(float maxValue);
    ShakeEffect(ShakeEffect &&) = default;
    ShakeEffect & operator =(ShakeEffect &&) = default;
    virtual ~ShakeEffect() = default;

    virtual void update(const sf::Time &) override;

private:
    float m_maxValue;
    std::default_random_engine m_rand;
};

#endif // SHAKEEFFECT_H

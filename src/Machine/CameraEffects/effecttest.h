#ifndef EFFECTTEST_H
#define EFFECTTEST_H

#include "cameraeffect.h"
#include <random>

class EffectTest : public CameraEffect
{
public:
    EffectTest();
    EffectTest(EffectTest &&) = default;
    EffectTest & operator =(EffectTest &&) = default;
    virtual ~EffectTest() = default;

    virtual void update(const sf::Time & elapsedTime) override;

private:
    std::default_random_engine m_engine;
    float m_totalTime;
};

#endif // EFFECTTEST_H

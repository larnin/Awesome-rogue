#ifndef FADEINEFFECT_H
#define FADEINEFFECT_H

#include "soundeffect.h"

class FadeinEffect : public SoundEffect
{
public:
    FadeinEffect(float delay, float fadein);
    FadeinEffect(FadeinEffect &&) = default;
    FadeinEffect & operator =(FadeinEffect &&) = default;
    virtual ~FadeinEffect() = default;

    virtual void update(const sf::Time & elapsedTime) override;

private:
    float m_delay;
    float m_fadein;
    float m_maxFadein;
};

#endif // FADEINEFFECT_H

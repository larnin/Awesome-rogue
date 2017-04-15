#ifndef FADEOUTEFFECT_H
#define FADEOUTEFFECT_H

#include "soundeffect.h"

class FadeoutEffect : public SoundEffect
{
public:
    FadeoutEffect(float delay, float fadeout);
    FadeoutEffect(FadeoutEffect &&) = default;
    FadeoutEffect & operator =(FadeoutEffect &&) = default;
    virtual ~FadeoutEffect() = default;

    virtual void update(const sf::Time & elapsedTime) override;

private:
    float m_delay;
    float m_fadeout;
    float m_maxFadeout;
};

#endif // FADEOUTEFFECT_H

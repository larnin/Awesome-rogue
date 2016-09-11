#include "fadeouteffect.h"

FadeoutEffect::FadeoutEffect(float delay, float fadeout)
    : m_delay(delay)
    , m_fadeout(fadeout)
    , m_maxFadeout(fadeout)

{

}

void FadeoutEffect::update(const sf::Time & elapsedTime)
{
    float time(elapsedTime.asSeconds());
    m_delay -= time;
    if(m_delay < 0)
    {
        m_fadeout -= m_delay;
        m_delay = 0;
    }
    if(m_delay <= 0 && m_maxFadeout <= 0)
    {
        m_finished = true;
        m_musicStopped = true;
        m_volumModifier = 0;
        return;
    }
    m_volumModifier = std::max(m_fadeout/m_maxFadeout, 0.0f);
    if(m_fadeout <= 0)
    {
        m_finished = true;
        m_musicStopped = true;
    }
}

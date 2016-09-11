#include "fadeineffect.h"

FadeinEffect::FadeinEffect(float delay, float fadein)
    : m_delay(delay)
    , m_fadein(0)
    , m_maxFadein(fadein)
{

}

void FadeinEffect::update(const sf::Time & elapsedTime)
{
    float time(elapsedTime.asSeconds());
    m_delay-= time;
    if(m_delay < 0)
    {
        m_fadein -= m_delay;
        m_delay = 0;
    }
    if(m_maxFadein <= 0 && m_delay <= 0)
    {
        m_volumModifier = 1;
        m_finished = true;
        return;
    }
    m_volumModifier = std::min(m_fadein/m_maxFadein, 1.0f);
    if(m_fadein >= m_maxFadein && m_delay <= 0)
        m_finished = true;
}

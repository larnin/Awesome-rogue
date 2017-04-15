#include "waitcondition.h"

WaitCondition::WaitCondition(float time)
    : m_totalTime(time)
    , m_currentTime(0)
{

}

bool WaitCondition::check(const Animator &, const sf::Time & elapsedTime)
{
    m_currentTime += elapsedTime.asSeconds();
    return m_currentTime >= m_totalTime;
}

void WaitCondition::reset()
{
    m_currentTime = 0;
}

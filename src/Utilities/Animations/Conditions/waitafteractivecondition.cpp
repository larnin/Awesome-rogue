#include "waitafteractivecondition.h"

WaitAfterActiveCondition::WaitAfterActiveCondition(std::unique_ptr<Condition> & condition, float time)
    : m_condition(std::move(condition))
    , m_active(false)
    , m_totalTime(time)
    , m_currentTime(0)
{

}

WaitAfterActiveCondition::WaitAfterActiveCondition(std::unique_ptr<Condition> && condition, float time)
    : m_condition(std::move(condition))
    , m_active(false)
    , m_totalTime(time)
    , m_currentTime(0)
{

}

bool WaitAfterActiveCondition::check(const Animator & animator, const sf::Time & elapsedTime)
{
    if(!m_condition)
        m_active = true;
    else if(m_condition->check(animator, elapsedTime))
        m_active = true;
    if(m_active)
        m_currentTime += elapsedTime.asSeconds();
    return m_currentTime >= m_totalTime;
}

void WaitAfterActiveCondition::reset()
{
    m_currentTime = 0;
    m_active = false;

    if(m_condition)
        m_condition->reset();
}

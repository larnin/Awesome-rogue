#include "notcondition.h"

NotCondition::NotCondition(std::unique_ptr<Condition> & condition)
    : m_condition(std::move(condition))
{

}

NotCondition::NotCondition(std::unique_ptr<Condition> && condition)
    : m_condition(std::move(condition))
{

}

bool NotCondition::check(const Animator & animator, const sf::Time &time)
{
    if(m_condition)
        return m_condition->check(animator, time);
    return true;
}

void NotCondition::reset()
{
    if(m_condition)
        m_condition->reset();
}

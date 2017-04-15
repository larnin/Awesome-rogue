#include "andcondition.h"
#include "Utilities/Animations/animator.h"

void AndCondition::add(std::unique_ptr<Condition> & condition)
{
    m_conditions.emplace_back(std::move(condition));
}

void AndCondition::add(std::unique_ptr<Condition> && condition)
{
    m_conditions.emplace_back(std::move(condition));
}

bool AndCondition::check(const Animator & animator, const sf::Time & time)
{
    bool ok = true;
    for(const auto & c : m_conditions)
    {
        if(!ok)
            break;
        if(!c)
            continue;
        ok = c->check(animator, time);
    }
    return ok;
}

void AndCondition::reset()
{
    for(auto & c : m_conditions)
    {
        if(c)
            c->reset();
    }
}

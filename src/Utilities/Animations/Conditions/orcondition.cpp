#include "orcondition.h"
#include "Utilities/Animations/animator.h"

void OrCondition::add(std::unique_ptr<Condition> & condition)
{
    m_conditions.emplace_back(std::move(condition));
}

void OrCondition::add(std::unique_ptr<Condition> && condition)
{
    m_conditions.emplace_back(std::move(condition));
}


bool OrCondition::check(const Animator & animator, const sf::Time & time)
{
    bool ok = false;
    for(const auto & c : m_conditions)
    {
        if(ok)
            break;
        if(!c)
            continue;
        ok = c->check(animator, time);
    }
    return ok;
}

void OrCondition::reset()
{
    for(auto & c : m_conditions)
    {
        if(c)
            c->reset();
    }
}

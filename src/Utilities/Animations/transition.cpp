#include "transition.h"
#include "animator.h"

Transition::Transition(const json & j)
{
    auto itCondition(j.find("condition"));
    if(itCondition != j.end())
        m_condition = std::move(Condition::load(*itCondition));
    auto itTargetAnimation(j.find("target"));
    if(itTargetAnimation != j.end())
        m_targetAnimationID = *itTargetAnimation;
}

Transition::Transition(unsigned int targetAnimationID, std::unique_ptr<Condition> && condition)
    : m_targetAnimationID(targetAnimationID)
    , m_condition(std::move(condition))
{

}

bool Transition::update(const Animator & animator, const sf::Time & elapsedTime)
{
    if(!m_condition)
        return false;
    return m_condition->check(animator, elapsedTime);
}

void Transition::reset()
{
    if(m_condition)
        m_condition.reset();
}

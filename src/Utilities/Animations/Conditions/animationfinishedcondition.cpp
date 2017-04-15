#include "animationfinishedcondition.h"
#include "Utilities/Animations/animator.h"

bool AnimationFinishedCondition::check(const Animator & animator, const sf::Time &)
{
    return animator.getCurrentAnimation().finished();
}

#ifndef ANIMATIONFINISHEDCONDITION_H
#define ANIMATIONFINISHEDCONDITION_H

#include "condition.h"

class AnimationFinishedCondition : public Condition
{
public:
    virtual ~AnimationFinishedCondition() = default;
    virtual bool check(const Animator & animator, const sf::Time &) override;
};

#endif // ANIMATIONFINISHEDCONDITION_H

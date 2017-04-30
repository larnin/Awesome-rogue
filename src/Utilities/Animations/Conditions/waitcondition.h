#ifndef WAITCONDITION_H
#define WAITCONDITION_H

#include "condition.h"

class WaitCondition : public Condition
{
public:
    WaitCondition(float time);
    virtual ~WaitCondition() = default;
    virtual bool check(const Animator &, const sf::Time & elapsedTime) override;
    virtual void reset() override;

private:
    float m_totalTime;
    float m_currentTime;
};

#endif // WAITCONDITION_H

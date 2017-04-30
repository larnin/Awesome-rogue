#ifndef NOTCONDITION_H
#define NOTCONDITION_H

#include "condition.h"
#include <memory>

class NotCondition : public Condition
{
public:
	virtual ~NotCondition() = default;
    NotCondition(std::unique_ptr<Condition> & condition);
    NotCondition(std::unique_ptr<Condition> && condition);
    virtual bool check(const Animator & animator, const sf::Time &time) override;
    virtual void reset() override;

private:
    std::unique_ptr<Condition> m_condition;
};

#endif // NOTCONDITION_H

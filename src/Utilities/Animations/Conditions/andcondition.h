#ifndef ANDCONDITION_H
#define ANDCONDITION_H

#include "condition.h"
#include <vector>
#include <memory>

class AndCondition : public Condition
{
public:
    virtual ~AndCondition() = default;
    void add(std::unique_ptr<Condition> & condition);
    void add(std::unique_ptr<Condition> && condition);
    virtual bool check(const Animator & animator, const sf::Time &time) override;
    virtual void reset() override;

private:
    std::vector<std::unique_ptr<Condition>> m_conditions;
};

#endif // ANDCONDITION_H

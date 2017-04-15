#ifndef ORCONDITION_H
#define ORCONDITION_H

#include "condition.h"
#include <vector>
#include <memory>

class OrCondition : public Condition
{
public:
    virtual ~OrCondition() = default;
    void add(std::unique_ptr<Condition> & condition);
    void add(std::unique_ptr<Condition> && condition);
    virtual bool check(const Animator & animator, const sf::Time &time) override;
    virtual void reset() override;

private:
    std::vector<std::unique_ptr<Condition>> m_conditions;
};

#endif // ORCONDITION_H

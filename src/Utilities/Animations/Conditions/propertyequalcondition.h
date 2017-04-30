#ifndef PROPERTYCONDITION_H
#define PROPERTYCONDITION_H

#include "condition.h"
#include <string>

class PropertyEqualCondition : public Condition
{
public:
    PropertyEqualCondition(const std::string & property, int value);
    virtual ~PropertyEqualCondition() = default;

    virtual bool check(const Animator & animator, const sf::Time &) override;

private:
    std::string m_property;
    int m_value;
};

#endif // PROPERTYCONDITION_H

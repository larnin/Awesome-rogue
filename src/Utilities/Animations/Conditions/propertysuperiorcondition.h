#ifndef PROPERTYSUPERIORCONDITION_H
#define PROPERTYSUPERIORCONDITION_H

#include "condition.h"
#include <string>

class PropertySuperiorCondition : public Condition
{
public:
    PropertySuperiorCondition(const std::string & property, int value);
    ~PropertySuperiorCondition() = default;

    virtual bool check(const Animator & animator, const sf::Time &) override;

private:
    std::string m_property;
    int m_value;
};

#endif // PROPERTYSUPERIORCONDITION_H

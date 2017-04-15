#ifndef PROPERTYSUPERIOROREQUALCONDITION_H
#define PROPERTYSUPERIOROREQUALCONDITION_H

#include "condition.h"
#include <string>

class PropertySuperiorOrEqualCondition : public Condition
{
public:
    PropertySuperiorOrEqualCondition(const std::string & property, int value);
    ~PropertySuperiorOrEqualCondition() = default;

    virtual bool check(const Animator & animator, const sf::Time &) override;

private:
    std::string m_property;
    int m_value;
};

#endif // PROPERTYSUPERIOROREQUALCONDITION_H

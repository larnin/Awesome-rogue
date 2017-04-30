#ifndef PROPERTYINFERIOROREQUALCONDITION_H
#define PROPERTYINFERIOROREQUALCONDITION_H

#include "condition.h"
#include <string>

class PropertyInferiorOrEqualCondition: public Condition
{
public:
    PropertyInferiorOrEqualCondition(const std::string & property, int value);
    virtual ~PropertyInferiorOrEqualCondition() = default;

    virtual bool check(const Animator & animator, const sf::Time &) override;

private:
    std::string m_property;
    int m_value;
};

#endif // PROPERTYINFERIOROREQUALCONDITION_H

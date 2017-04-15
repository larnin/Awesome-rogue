#ifndef PROPERTYINFERIORCONDITION_H
#define PROPERTYINFERIORCONDITION_H

#include "condition.h"
#include <string>

class PropertyInferiorCondition : public Condition
{
public:
    PropertyInferiorCondition(const std::string & property, int value);
    ~PropertyInferiorCondition() = default;

    virtual bool check(const Animator & animator, const sf::Time &) override;

private:
    std::string m_property;
    int m_value;
};

#endif // PROPERTYINFERIORCONDITION_H

#ifndef PROPERTYNOTEQUALCONDITION_H
#define PROPERTYNOTEQUALCONDITION_H

#include "condition.h"
#include <string>

class PropertyNotEqualCondition: public Condition
{
public:
    PropertyNotEqualCondition(const std::string & property, int value);
    virtual ~PropertyNotEqualCondition() = default;

    virtual bool check(const Animator & animator, const sf::Time &) override;

private:
    std::string m_property;
    int m_value;
};

#endif // PROPERTYNOTEQUALCONDITION_H

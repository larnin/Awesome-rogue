#include "propertysuperiorcondition.h"
#include "Utilities/Animations/animator.h"

PropertySuperiorCondition::PropertySuperiorCondition(const std::string & property, int value)
    : m_property(property)
    , m_value(value)
{

}

bool PropertySuperiorCondition::check(const Animator & animator, const sf::Time &)
{
    return animator.getProperty(m_property) > m_value;
}

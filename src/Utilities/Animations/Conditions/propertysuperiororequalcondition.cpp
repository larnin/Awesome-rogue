#include "propertysuperiororequalcondition.h"
#include "Utilities/Animations/animator.h"

PropertySuperiorOrEqualCondition::PropertySuperiorOrEqualCondition(const std::string & property, int value)
    : m_property(property)
    , m_value(value)
{

}

bool PropertySuperiorOrEqualCondition::check(const Animator & animator, const sf::Time &)
{
    return animator.getProperty(m_property) >= m_value;
}

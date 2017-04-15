#include "propertyequalcondition.h"
#include "Utilities/Animations/animator.h"

PropertyEqualCondition::PropertyEqualCondition(const std::string & property, int value)
    : m_property(property)
    , m_value(value)
{

}

bool PropertyEqualCondition::check(const Animator & animator, const sf::Time &)
{
    return animator.getProperty(m_property) == m_value;
}

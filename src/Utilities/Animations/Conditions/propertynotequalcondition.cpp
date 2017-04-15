#include "propertynotequalcondition.h"
#include "Utilities/Animations/animator.h"

PropertyNotEqualCondition::PropertyNotEqualCondition(const std::string & property, int value)
    : m_property(property)
    , m_value(value)
{

}

bool PropertyNotEqualCondition::check(const Animator & animator, const sf::Time &)
{
    return animator.getProperty(m_property) != m_value;
}

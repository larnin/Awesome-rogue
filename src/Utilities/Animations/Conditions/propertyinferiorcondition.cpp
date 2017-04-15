#include "propertyinferiorcondition.h"
#include "Utilities/Animations/animator.h"

PropertyInferiorCondition::PropertyInferiorCondition(const std::string & property, int value)
    : m_property(property)
    , m_value(value)
{

}

bool PropertyInferiorCondition::check(const Animator & animator, const sf::Time &)
{
    return animator.getProperty(m_property) < m_value;
}

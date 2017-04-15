#include "propertyinferiororequalcondition.h"
#include "Utilities/Animations/animator.h"

PropertyInferiorOrEqualCondition::PropertyInferiorOrEqualCondition(const std::string & property, int value)
    : m_property(property)
    , m_value(value)
{

}

bool PropertyInferiorOrEqualCondition::check(const Animator & animator, const sf::Time &)
{
    return animator.getProperty(m_property) <= m_value;
}

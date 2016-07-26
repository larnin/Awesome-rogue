#include "shakeeffect.h"

ShakeEffect::ShakeEffect(float maxValue)
    : m_maxValue(maxValue)
{

}

void ShakeEffect::update(const sf::Time &)
{
    std::uniform_real_distribution<float> dOffset(-m_maxValue, m_maxValue);
    m_offset = sf::Vector2f(dOffset(m_rand), dOffset(m_rand));
}

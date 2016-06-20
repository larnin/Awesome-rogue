#include "effecttest.h"
#include "Utilities/vect2convert.h"

EffectTest::EffectTest()
{

}

void EffectTest::update(const sf::Time & elapsedTime)
{
    const float angleBySecond(30.0f);
    const float maxRandomShake(2.0f);
    const float rotationOffsetSpeed(8.0f);
    const float rotationOffsetDistance(20.0f);
    const float zoomSpeed(2.0f);
    const float zoomAmplitude(0.2f);

    m_totalTime += elapsedTime.asSeconds();

    m_orientation  = m_totalTime*angleBySecond;
    std::uniform_real_distribution<float> distrib(-maxRandomShake, maxRandomShake);
    sf::Vector2f randomShake(distrib(m_engine), distrib(m_engine));
    m_offset = randomShake + toVect(rotationOffsetDistance, rotationOffsetSpeed*m_totalTime);
    m_zoom = std::sin(m_totalTime*zoomSpeed)*zoomAmplitude + 1;
}

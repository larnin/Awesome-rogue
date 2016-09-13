#ifndef CIRCLECOLOREDLIGHT_H
#define CIRCLECOLOREDLIGHT_H

#include "light.h"
#include "Utilities/ressource.h"
#include <SFML/Graphics/Color.hpp>

class CircleColoredLight : public Light
{
public:
    CircleColoredLight(const sf::Vector2f & pos, const sf::Color & base, float baseSize, const sf::Color & saturation, float saturationSize);
    CircleColoredLight(CircleColoredLight &&) = default;
    CircleColoredLight & operator=(CircleColoredLight &&) = default;
    virtual ~CircleColoredLight() = default;

    virtual void drawBase(sf::RenderTarget & target) const;
    virtual void drawSaturation(sf::RenderTarget & target) const;

private:
    Texture m_texture;
    sf::Color m_baseColor;
    sf::Color m_saturationColor;
    float m_baseSize;
    float m_saturationSize;
};

#endif // CIRCLECOLOREDLIGHT_H

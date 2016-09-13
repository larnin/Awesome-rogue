#include "circlecoloredlight.h"
#include "Utilities/quadrender.h"
#include <SFML/Graphics/VertexArray.hpp>

CircleColoredLight::CircleColoredLight(const sf::Vector2f & pos, const sf::Color & base, float baseSize, const sf::Color & saturation, float saturationSize)
    : Light(pos)
    , m_texture("res/img/light_fx.png")
    , m_baseColor(base)
    , m_saturationColor(saturation)
    , m_baseSize(baseSize)
    , m_saturationSize(saturationSize)
{

}

void CircleColoredLight::drawBase(sf::RenderTarget & target) const
{
    sf::VertexArray array(sf::Quads, 4);
    drawQuad(&array[0], sf::FloatRect(m_pos.x-m_baseSize, m_pos.y-m_baseSize, m_baseSize*2, m_baseSize*2)
            , sf::FloatRect(0, 0, m_texture->getSize().x, m_texture->getSize().y));
    for(unsigned int i(0) ; i < array.getVertexCount() ; i++)
        array[i].color = m_baseColor;
    sf::RenderStates s;
    s.blendMode = sf::BlendAdd;
    s.texture = m_texture();
    target.draw(array, s);
}

void CircleColoredLight::drawSaturation(sf::RenderTarget & target) const
{
    sf::VertexArray array(sf::Quads, 4);
    drawQuad(&array[0], sf::FloatRect(m_pos.x-m_saturationSize, m_pos.y-m_saturationSize, m_saturationSize*2, m_saturationSize*2)
            , sf::FloatRect(0, 0, m_texture->getSize().x, m_texture->getSize().y));
    for(unsigned int i(0) ; i < array.getVertexCount() ; i++)
        array[i].color = m_saturationColor;
    sf::RenderStates s;
    s.blendMode = sf::BlendAdd;
    s.texture = m_texture();
    target.draw(array, s);
}

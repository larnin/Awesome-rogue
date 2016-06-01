#include "lifebar.h"
#include "Utilities/quadrender.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Rect.hpp>

LifeBar::LifeBar(std::weak_ptr<Entity> e)
    : m_entity(e)
    , m_texture("res/img/lifebar.png")
{

}

void LifeBar::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    const sf::FloatRect borderLife(0, 0, 55, 10);
    const sf::FloatRect borderShield(0, 20, 55, 9);
    sf::FloatRect fillLife(0, 10, 45, 10);
    sf::FloatRect fillShield(0, 29, 46, 9);

    const sf::Vector2f posLife(7, 23);
    const sf::Vector2f posShield(7, 17);

    std::shared_ptr<Entity> e(m_entity.lock());
    sf::VertexArray array(sf::Quads, 16);

    const sf::View & v(target.getView());
    sf::Vector2f vSize(v.getSize());
    sf::Vector2f vTopLeft(v.getCenter()-vSize/2.0f);

    if(e)
        if(e->getMaxShield() > 0)
        {
            fillShield.width *= e->getShield()/e->getMaxShield();
            drawQuad(&array[0]
                    , sf::FloatRect(vTopLeft + sf::Vector2f(posShield.x, vSize.y - posShield.y), sf::Vector2f(borderShield.width, borderShield.height))
                    , borderShield);

            drawQuad(&array[4]
                    , sf::FloatRect(vTopLeft + sf::Vector2f(posShield.x, vSize.y - posShield.y), sf::Vector2f(fillShield.width, fillShield.height))
                    , fillShield);
            array[6].position.x += 9;
            array[6].texCoords.x += 9;
            array[7].position.x += 9;
            array[7].texCoords.x += 9;
        }
    drawQuad(&array[8]
            , sf::FloatRect(vTopLeft + sf::Vector2f(posLife.x, vSize.y - posLife.y), sf::Vector2f(borderLife.width, borderLife.height))
            , borderLife);
    if(e)
    {
        fillLife.width*= e->getLife()/e->getMaxLife();
        drawQuad(&array[12]
                , sf::FloatRect(vTopLeft + sf::Vector2f(posLife.x, vSize.y - posLife.y), sf::Vector2f(fillLife.width, fillLife.height))
                , fillLife);
        array[14].position.x += 9;
        array[14].texCoords.x += 9;
        array[15].position.x += 9;
        array[15].texCoords.x += 9;
    }

    target.draw(array, sf::RenderStates(m_texture()));
}

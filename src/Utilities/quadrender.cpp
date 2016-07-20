#include "quadrender.h"
#include "Utilities/vect2convert.h"
#include "Map/block.h"
#include "Collisions/hitbox.h"

void drawQuad(sf::Vertex* quads, const sf::FloatRect & rect, const sf::FloatRect & texRect, bool xFliped, bool yFliped, Rotation rot)
{
    quads[0] = sf::Vertex(sf::Vector2f(rect.left, rect.top), sf::Vector2f(texRect.left, texRect.top));
    quads[1] = sf::Vertex(sf::Vector2f(rect.left+rect.width, rect.top), sf::Vector2f(texRect.left+texRect.width, texRect.top));
    quads[2] = sf::Vertex(sf::Vector2f(rect.left+rect.width, rect.top+rect.height), sf::Vector2f(texRect.left+texRect.width, texRect.top+texRect.height));
    quads[3] = sf::Vertex(sf::Vector2f(rect.left, rect.top+rect.height), sf::Vector2f(texRect.left, texRect.top+texRect.height));

    for(unsigned int i(0) ; i < rot ; i++)
    {
        sf::Vector2f texTemp(quads[0].texCoords);
        quads[0].texCoords = quads[3].texCoords;
        quads[3].texCoords = quads[2].texCoords;
        quads[2].texCoords = quads[1].texCoords;
        quads[1].texCoords = texTemp;
    }

    if(xFliped)
    {
        std::swap(quads[0].texCoords, quads[1].texCoords);
        std::swap(quads[2].texCoords, quads[3].texCoords);
    }

    if(yFliped)
    {
        std::swap(quads[0].texCoords, quads[3].texCoords);
        std::swap(quads[1].texCoords, quads[2].texCoords);
    }
}

void drawOrientedQuad(sf::Vertex * quads, const sf::FloatRect & rect, const sf::FloatRect & texRect, bool xFliped, bool yFliped, Rotation rot
                      , float rotation, const sf::Vector2f & rotationOrigine)
{
    drawQuad(quads, rect, texRect, xFliped, yFliped, rot);
    for(int i(0) ; i < 4 ; i++)
    {
        sf::Vector2f & pos(quads[i].position);
        sf::Vector2f vect(rotationOrigine - pos);
        sf::Vector2f newPos(rotationOrigine + toVect(norm(vect), angle(vect) + rotation));
        pos.x = newPos.x;
        pos.y = newPos.y;
    }
}

void drawFlatQuad(sf::Vertex* quads, const sf::FloatRect & rect, const sf::Color & color)
{
    quads[0] = sf::Vertex(sf::Vector2f(rect.left, rect.top), color);
    quads[1] = sf::Vertex(sf::Vector2f(rect.left+rect.width, rect.top), color);
    quads[2] = sf::Vertex(sf::Vector2f(rect.left+rect.width, rect.top+rect.height), color);
    quads[3] = sf::Vertex(sf::Vector2f(rect.left, rect.top+rect.height), color);
}


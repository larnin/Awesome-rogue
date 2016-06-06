#include "gameinterface.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "Utilities/quadrender.h"

GameInterface::GameInterface()
    : m_texture("res/img/interface.png")
{

}

void GameInterface::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    const sf::FloatRect texLifeHolder(0, 50, 86, 35);
    const sf::FloatRect texCenter(86, 76, 2, 9);
    const sf::FloatRect texmapHolder(88, 7, 132, 78);

    const sf::View & v(target.getView());

    sf::VertexArray array(sf::Quads, 12);
    sf::Vector2f vSize(v.getSize());
    sf::Vector2f vTopLeft(v.getCenter()-vSize/2.0f);

    drawQuad(&array[0], sf::FloatRect(vTopLeft.x, vTopLeft.y+vSize.y-texLifeHolder.height, texLifeHolder.width, texLifeHolder.height), texLifeHolder);
    drawQuad(&array[4], sf::FloatRect(vTopLeft.x+vSize.x-texmapHolder.width, vTopLeft.y+vSize.y-texmapHolder.height, texmapHolder.width, texmapHolder.height), texmapHolder);
    drawQuad(&array[8], sf::FloatRect(vTopLeft.x+texLifeHolder.width, vTopLeft.y+vSize.y-texCenter.height, vSize.x-texLifeHolder.width-texmapHolder.width, texCenter.height), texCenter);

    target.draw(array, sf::RenderStates(m_texture()));
}
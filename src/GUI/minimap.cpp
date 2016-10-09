#include "minimap.h"
#include "Map/blocktype.h"
#include "Utilities/quadrender.h"
#include "Events/Datas/eventplayerchangeroom.h"
#include "Map/map.h"
#include "Map/room.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

float mapScale(0.75f);
float mapOverResolution(3.0f);

Minimap::Minimap(std::weak_ptr<Map> map)
    : m_map(map)
    , m_array(sf::Quads)
{
    m_texture.create(62/mapScale*mapOverResolution, 62/mapScale*mapOverResolution);
    m_texture.setSmooth(true);
    sf::View v(m_texture.getView());
    v.zoom(1/mapOverResolution);
    m_texture.setView(v);

    redraw();
    connect<EventPlayerChangeRoom>(std::bind(&Minimap::onPlayerChangeRoom, this, _1));
}

void Minimap::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    const sf::View & v(target.getView());
    sf::Vector2f pos(v.getCenter() + v.getSize()/2.0f -sf::Vector2f(65, 67));

    m_texture.clear(sf::Color(195, 195, 195));
    sf::View texView(m_texture.getView());
    texView.setCenter(v.getCenter()/float(BlockType::tileSize));
    m_texture.setView(texView);
    m_texture.draw(m_array);
    m_texture.display();

    sf::Sprite mapRender(m_texture.getTexture());
    mapRender.setPosition(pos);
    mapRender.setScale(mapScale/mapOverResolution, mapScale/mapOverResolution);
    target.draw(mapRender);
}

void Minimap::onPlayerChangeRoom(EventPlayerChangeRoom)
{
    redraw();
}

void Minimap::redraw()
{
    m_array.clear();
    std::shared_ptr<Map> m(m_map.lock());
    if(!m)
        return;

    const sf::Color border(0, 162, 232);
    const sf::Color center(221, 243, 249);
    const sf::Color door(153, 217, 234);

    for(const std::shared_ptr<Room> & r : *m)
    {
        if(!r)
            continue;
        if(!r->isDiscovered())
            continue;
        auto arraySize(m_array.getVertexCount());
        m_array.resize(arraySize+8+r->doors().size()*4);
        sf::Vector2i pos(r->getPos());
        sf::Vector2u size(r->getSize());
        sf::Vector2u smallSize(size.x-2, size.y-2);
        sf::Vector2i smallPos(pos.x+1, pos.y+1);

        drawFlatQuad(&m_array[arraySize], sf::FloatRect(sf::Vector2f(pos), sf::Vector2f(size)), border);
        drawFlatQuad(&m_array[arraySize+4], sf::FloatRect(sf::Vector2f(smallPos), sf::Vector2f(smallSize)), center);

        unsigned int doorID(0);
        for(const Door & d : r->doors())
        {
            drawFlatQuad(&m_array[arraySize+8+doorID*4], sf::FloatRect(sf::Vector2f(d.pos.toGlobalBlockPos()), sf::Vector2f(1, 1)), door);
            doorID++;
        }
    }
}

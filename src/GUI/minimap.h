#ifndef MINIMAP_H
#define MINIMAP_H

#include <memory>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include "Map/map.h"
#include "Events/eventreceiver.h"
#include "Events/Datas/evententitychangeroom.h"

class Minimap : public sf::Drawable, public EventReceiver
{
public:
    Minimap(std::weak_ptr<Map> map);
    virtual ~Minimap() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    void onEntityChangeRoom(EventEntityChangeRoom);
    void redraw();

    std::weak_ptr<Map> m_map;
    mutable sf::RenderTexture m_texture;
    sf::VertexArray m_array;
};

#endif // MINIMAP_H

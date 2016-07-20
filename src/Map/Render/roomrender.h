#ifndef ROOMRENDER_H
#define ROOMRENDER_H

#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "Utilities/ressource.h"

class Room;
class Block;

class RoomRender : public sf::Drawable
{
public:
    RoomRender(std::weak_ptr<Room> room, bool current);
    virtual ~RoomRender() = default;
    void redraw(bool current) const;
    std::weak_ptr<Room> getRoom() const;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    unsigned int getNbSurfaces() const;
    unsigned int drawBlock(sf::Vertex* quads, const Block & b, const sf::Vector2i & globalPos) const;
    mutable sf::VertexArray m_render;
    std::weak_ptr<Room> m_room;
    Texture m_texture;
    mutable bool m_current;
};

#endif // ROOMRENDER_H

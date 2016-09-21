#ifndef ROOMRENDER_H
#define ROOMRENDER_H

#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "Utilities/ressource.h"
#include "Systemes/updatable.h"
#include "renderdata.h"

class Room;
class Block;

class RoomRender : public sf::Drawable, public Updatable
{
public:
    RoomRender(std::weak_ptr<Room> room, bool current);
    virtual ~RoomRender() = default;
    void redraw(bool current) const;
    std::weak_ptr<Room> getRoom() const;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;
    virtual void update(const sf::Time & elapsedTime);

private:
    struct BlockAnimationState
    {
        BlockAnimationState(sf::Vertex *_quad, unsigned int _id, unsigned char _orientation, sf::Vector2u _pos)
            : quad(_quad), id(_id), orientation(_orientation), animID(_id), pos(_pos)
        {}

        sf::Vertex* quad;
        unsigned int id;
        unsigned char orientation;
        unsigned int animID;
        sf::Vector2u pos;
    };

    unsigned int getNbSurfaces() const;
    unsigned int drawBlock(sf::Vertex* quads, const Block & b, const sf::Vector2i & globalPos) const;
    mutable sf::VertexArray m_render;
    std::weak_ptr<Room> m_room;
    mutable bool m_current;
    RenderData m_data;
    mutable std::vector<BlockAnimationState> m_animation;
    float m_time;
};

#endif // ROOMRENDER_H

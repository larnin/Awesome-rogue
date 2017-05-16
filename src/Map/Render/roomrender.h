#ifndef ROOMRENDER_H
#define ROOMRENDER_H

#include "Utilities/ressource.h"
#include "Systemes/updatable.h"
#include "renderdata.h"
#include "Lights/phongshader.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <memory>

class Room;
class Block;

class RoomRender : public sf::Drawable, public Updatable
{
public:
    RoomRender(std::weak_ptr<Room> room, bool current, bool drawGround, bool drawWall, bool drawTop);
    virtual ~RoomRender() = default;
    void redraw(bool current) const;
    std::weak_ptr<Room> getRoom() const;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;
    virtual void update(const sf::Time & elapsedTime) override;

private:
    struct BlockAnimationState
    {
        BlockAnimationState(unsigned int _index, unsigned int _id, unsigned char _orientation, sf::Vector2u _pos)
            : index(_index), id(_id), orientation(_orientation), pos(_pos)
        {}

        unsigned int index;
        unsigned int id;
        unsigned char orientation;
        sf::Vector2u pos;
    };

    unsigned int getNbSurfaces() const;
    unsigned int drawBlock(sf::Vertex* quads, const Block & b, const sf::Vector2i & globalPos) const;
    mutable sf::VertexArray m_render;
    std::weak_ptr<Room> m_room;
    mutable bool m_current;
    RenderData m_data;
    mutable std::vector<BlockAnimationState> m_animation;

    bool m_drawGround;
    bool m_drawWall;
    bool m_drawTop;

    PhongShader m_shader;
};

#endif // ROOMRENDER_H

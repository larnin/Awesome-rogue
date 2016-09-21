#include "roomrender.h"
#include "Utilities/quadrender.h"
#include "Map/blocktype.h"
#include "Map/room.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

RoomRender::RoomRender(std::weak_ptr<Room> room, bool current)
    : m_render(sf::Quads)
    , m_room(room)
    , m_data([room]() -> std::string
    {
        auto r(room.lock());
        if(!r)
            return "";
        return r->getRenderInfosName();
    }())
    , m_time(0)
{
    redraw(current);
}

void RoomRender::redraw(bool current) const
{
    const unsigned int flatWhiteID(30);

    m_current = current;
    std::shared_ptr<Room> r(m_room.lock());
    if(!r)
    {
        m_render.resize(0);
        return;
    }

    m_animation.clear();
    r->modified = false;

    if(r->isDiscovered())
    {
        m_render.resize((getNbSurfaces()+(current?0:1))*4);

        int index(0);
        for(unsigned int i(0) ; i < r->getSize().x ; i++)
            for(unsigned int j(0) ; j < r->getSize().y ; j++)
            {
                Block b((*r)(sf::Vector2u(i, j)));
                if(m_data.hasAnimation(b.groundID))
                {
                    m_animation.push_back(BlockAnimationState(&m_render[index*4], b.groundID, b.groundOrientation, sf::Vector2u(i, j)));
                    b.groundID = m_data.frameOf(b.groundID, m_time);
                }
                if(m_data.hasAnimation(b.wallID))
                {
                    m_animation.push_back(BlockAnimationState(&m_render[(index+1)*4], b.wallID, b.wallOrientation, sf::Vector2u(i, j)));
                    b.wallID = m_data.frameOf(b.wallID, m_time);
                }
                index += drawBlock(&m_render[index*4], b, sf::Vector2i(i, j)+r->getPos());
            }
        if(!current)
        {
            const unsigned int startIndex(m_render.getVertexCount()-4);
            sf::Vector2f texPos(flatWhiteID%BlockType::nbTile*BlockType::tileSize, flatWhiteID/BlockType::nbTile*BlockType::tileSize);
            drawFlatQuad(&m_render[startIndex]
                    , sf::FloatRect((sf::Vector2f(r->getPos())-sf::Vector2f(0.5f, 0.5f))*float(BlockType::tileSize), sf::Vector2f(r->getSize())*float(BlockType::tileSize))
                    , sf::Color(0, 0, 0, 100));
            m_render[startIndex].texCoords = texPos;
            m_render[startIndex+1].texCoords = sf::Vector2f(texPos.x, texPos.y+BlockType::tileSize);
            m_render[startIndex+2].texCoords = sf::Vector2f(texPos.x+BlockType::tileSize, texPos.y+BlockType::tileSize);
            m_render[startIndex+3].texCoords = sf::Vector2f(texPos.x+BlockType::tileSize, texPos.y);
        }
    }
    else
    {
        m_render.resize(8);
        sf::Vector2f texPos(flatWhiteID%BlockType::nbTile*BlockType::tileSize, flatWhiteID/BlockType::nbTile*BlockType::tileSize);
        drawFlatQuad(&m_render[0]
                , sf::FloatRect((sf::Vector2f(r->getPos())-sf::Vector2f(0.5f, 0.5f))*float(BlockType::tileSize), sf::Vector2f(r->getSize())*float(BlockType::tileSize))
                , sf::Color(33, 31, 35));
        drawFlatQuad(&m_render[4]
                , sf::FloatRect((sf::Vector2f(r->getPos())+sf::Vector2f(0.5f, 0.5f))*float(BlockType::tileSize), (sf::Vector2f(r->getSize())-sf::Vector2f(2, 2))*float(BlockType::tileSize))
                , sf::Color(22, 20, 24));
        for(unsigned int i(0) ; i < 2 ; i++)
        {
            m_render[4*i].texCoords = texPos;
            m_render[4*i+1].texCoords = sf::Vector2f(texPos.x, texPos.y+BlockType::tileSize);
            m_render[4*i+2].texCoords = sf::Vector2f(texPos.x+BlockType::tileSize, texPos.y+BlockType::tileSize);
            m_render[4*i+3].texCoords = sf::Vector2f(texPos.x+BlockType::tileSize, texPos.y);
        }
    }
}

void RoomRender::draw(sf::RenderTarget & target, sf::RenderStates) const
{    std::shared_ptr<Room> r(m_room.lock());
     if(!r)
         return;

    if(r->modified)
        redraw(m_current);

    if(m_data.texture.isValid())
        target.draw(m_render, sf::RenderStates(m_data.texture()));
}

unsigned int RoomRender::getNbSurfaces() const
{
    std::shared_ptr<Room> r(m_room.lock());
    if(!r)
        return 0;

    int nbDrawable(0);
    for(unsigned int i(0) ; i < r->getSize().x ; i++)
        for(unsigned int j(0) ; j < r->getSize().y ; j++)
        {
            Block b((*r)(sf::Vector2u(i, j)));
            if(b.groundID != 0)
                nbDrawable++;
            if(b.wallID != 0)
                nbDrawable++;
        }
    return nbDrawable;
}

unsigned int RoomRender::drawBlock(sf::Vertex* quads, const Block & b, const sf::Vector2i & globalPos) const
{
    unsigned int offset(0);

    if(b.groundID != 0)
    {
        sf::Vector2f texPos(b.groundID%BlockType::nbTile, b.groundID/BlockType::nbTile);
        drawQuad(quads, sf::FloatRect(sf::Vector2f(globalPos)*float(BlockType::tileSize)
                                      -sf::Vector2f(BlockType::tileSize, BlockType::tileSize)/2.0f, sf::Vector2f(BlockType::tileSize, BlockType::tileSize))
                 , sf::FloatRect(texPos*float(BlockType::tileSize), sf::Vector2f(BlockType::tileSize, BlockType::tileSize))
                 , getXFlip(b.groundOrientation), getYFlip(b.groundOrientation), getRotation(b.groundOrientation));
        offset++;
    }
    if(b.wallID != 0)
    {
        sf::Vector2f texPos(b.wallID%BlockType::nbTile, b.wallID/BlockType::nbTile);
        drawQuad(quads+4*offset, sf::FloatRect(sf::Vector2f(globalPos)*float(BlockType::tileSize)
                                    -sf::Vector2f(BlockType::tileSize, BlockType::tileSize)/2.0f, sf::Vector2f(BlockType::tileSize, BlockType::tileSize))
                 , sf::FloatRect(texPos*float(BlockType::tileSize), sf::Vector2f(BlockType::tileSize, BlockType::tileSize))
                 , getXFlip(b.wallOrientation), getYFlip(b.wallOrientation), getRotation(b.wallOrientation));
        offset++;
    }

    return offset;
}

std::weak_ptr<Room> RoomRender::getRoom() const
{
    return m_room;
}

void RoomRender::update(const sf::Time & elapsedTime)
{
    m_time += elapsedTime.asSeconds();
    std::shared_ptr<Room> r(m_room.lock());
    if(!r)
        return;

    for(BlockAnimationState & a : m_animation)
    {
        unsigned int id(m_data.frameOf(a.id, m_time));
        if(id == a.animID)
            continue;
        a.animID = id;

        sf::Vector2f texPos(a.animID%BlockType::nbTile, a.animID/BlockType::nbTile);
        sf::Vector2f globalPos((sf::Vector2f(r->getPos())+sf::Vector2f(a.pos))*float(BlockType::tileSize));
        drawQuad(a.quad, sf::FloatRect(sf::Vector2f(globalPos)*float(BlockType::tileSize)
                                    -sf::Vector2f(BlockType::tileSize, BlockType::tileSize)/2.0f, sf::Vector2f(BlockType::tileSize, BlockType::tileSize))
                 , sf::FloatRect(texPos*float(BlockType::tileSize), sf::Vector2f(BlockType::tileSize, BlockType::tileSize))
                 , getXFlip(a.orientation), getYFlip(a.orientation), getRotation(a.orientation));
    }
}

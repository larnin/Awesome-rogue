#include "location.h"
#include "room.h"

Location::Location(const sf::Vector2u & blockPos, std::weak_ptr<Room> room)
    : m_pos(blockPos)
    , m_room(room)
    , m_roomID(0)
{

}

Location::Location(const sf::Vector2i &blockPos, std::weak_ptr<Room> room)
    : m_pos(blockPos)
    , m_room(room)
    , m_roomID(0)
{

}

Location::Location(const sf::Vector2f & pos, std::weak_ptr<Room> room)
    : m_pos(pos)
    , m_room(room)
    , m_roomID(0)
{

}

Location::Location(const sf::Vector2f & pos, unsigned int roomID)
    : m_pos(pos)
    , m_roomID(roomID)
{

}

bool Location::isValide() const
{
    std::shared_ptr<Room> r(m_room.lock());
    if(!r)
        return false;
    if(m_pos.x < 0 || m_pos.y < 0)
        return false;
    sf::Vector2u blockPos(getBlockPos());
    if(blockPos.x >= r->getSize().x || blockPos.y >= r->getSize().y)
        return false;
    return true;
}

const std::weak_ptr<Room> & Location::getRoom() const
{
    return m_room;
}

unsigned int Location::getRoomID() const
{
    auto r(m_room.lock());
    if(r)
        return r->getID();
    return m_roomID;
}

sf::Vector2f Location::getPos() const
{
    return m_pos;
}

sf::Vector2u Location::getBlockPos() const
{
    sf::Vector2u blockPos;
    if(m_pos.x > 0)
        blockPos.x = m_pos.x + 0.5f;
    if(m_pos.y > 0)
        blockPos.y = m_pos.y + 0.5f;
    return blockPos;
}

sf::Vector2f Location::toGlobalPos() const
{
    std::shared_ptr<Room> r(m_room.lock());
    if(!r)
        return getPos();
    return getPos() + sf::Vector2f(r->getPos());
}

sf::Vector2i Location::toGlobalBlockPos() const
{
    std::shared_ptr<Room> r(m_room.lock());
    if(!r)
        return sf::Vector2i(getBlockPos());
    return sf::Vector2i(getBlockPos()) + r->getPos();
}

void Location::move(sf::Vector2f dir)
{
    m_pos += dir;
}

Location Location::fromGlobal(const sf::Vector2f & pos, std::weak_ptr<Room> room)
{
    std::shared_ptr<Room> r(room.lock());
    if(!r)
        return Location(pos);
    return Location(pos-sf::Vector2f(r->getPos()), room);
}

Location Location::fromGlobalBlock(const sf::Vector2i & pos, std::weak_ptr<Room> room)
{
    std::shared_ptr<Room> r(room.lock());
    if(!r)
        return Location(sf::Vector2f(pos));
    return Location(sf::Vector2f(pos-r->getPos()), room);
}

sf::Vector2i Location::posToBlockPos(const sf::Vector2f & pos)
{
    sf::Vector2i blockPos;
    blockPos.x = pos.x > 0 ? pos.x + 0.5f : pos.x - 0.5f;
    blockPos.y = pos.y > 0 ? pos.y + 0.5f : pos.y - 0.5f;
    return blockPos;
}

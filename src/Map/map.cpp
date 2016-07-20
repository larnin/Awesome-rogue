#include <algorithm>
#include "map.h"
#include "room.h"

Map::Map()
{

}

void Map::addRoom(const std::shared_ptr<Room> & r)
{
    if(!r)
        return;
    auto it(std::find(m_rooms.begin(), m_rooms.end(), r));
    if(it != m_rooms.end())
        return;
    m_rooms.push_back(r);
}

void Map::removeRoom(const std::shared_ptr<Room> & r)
{
    auto it(std::find(m_rooms.begin(), m_rooms.end(), r));
    if(it == m_rooms.end())
        return;
    std::swap(*it, m_rooms.back());
    m_rooms.pop_back();
}

unsigned int Map::count() const
{
    return m_rooms.size();
}

std::shared_ptr<Room> Map::roomAt(const sf::Vector2i & pos)
{
    auto it(std::find_if(m_rooms.begin(), m_rooms.end(), [pos](const std::shared_ptr<Room> & r)
        {
            if(!r)
                return false;
            return pos.x >= r->getPos().x && pos.y >= r->getPos().y && pos.x < r->getPos().x+int(r->getSize().x) && pos.y < r->getPos().y+int(r->getSize().y);
        }));
    if(it == m_rooms.end())
        return std::shared_ptr<Room>();
    return *it;
}

std::shared_ptr<Room> Map::room(unsigned int id)
{
    auto it(std::find_if(m_rooms.begin(), m_rooms.end(), [id](const std::shared_ptr<Room> & r)
        {
            if(!r)
                return false;
            return r->getID() == id;
        }));
    if(it == m_rooms.end())
        return std::shared_ptr<Room>();
    return *it;
}

std::vector<std::shared_ptr<Room>>::iterator Map::begin()
{
    return m_rooms.begin();
}

std::vector<std::shared_ptr<Room>>::iterator Map::end()
{
    return m_rooms.end();
}

std::vector<std::shared_ptr<Room>>::const_iterator Map::begin() const
{
    return m_rooms.cbegin();
}

std::vector<std::shared_ptr<Room>>::const_iterator Map::end() const
{
    return m_rooms.cend();
}

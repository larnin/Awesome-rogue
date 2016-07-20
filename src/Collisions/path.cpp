#include <algorithm>
#include "path.h"
#include "pathfinder.h"
#include "Utilities/vect2convert.h"
#include "Map/room.h"

Path::Path()
{

}

Path::Path(const Location & pos, const Location & dest)
{
    newPath(pos, dest);
}

Location Path::dest() const
{
    return m_dest;
}

void Path::newPath(const Location & pos, const Location & dest)
{
    m_dest = dest;
    m_actualRoom = pos.getRoom();

    m_posPath.clear();
    m_roomPath.clear();

    std::shared_ptr<Room> posRoom(pos.getRoom().lock());
    std::shared_ptr<Room> endRoom(dest.getRoom().lock());
    if(!posRoom || !endRoom)
        return;

    if(posRoom != endRoom)
        m_roomPath = PathFinder::pathRooms(posRoom, endRoom);
    pathToDoor(pos);
}

Location Path::next(const Location & actual)
{
    if(finished())
        return actual;
    if(actual.getRoom().lock() != m_actualRoom.lock())
    {
        std::shared_ptr<Room> next;
        if(m_roomPath.empty())
            next = m_dest.getRoom().lock();
        else next = m_roomPath.front().lock();

        if(next != actual.getRoom().lock())
        {
            clear();
            return actual;
        }
        newPath(actual, m_dest);
    }
    if(m_posPath.empty())
    {
        clear();
        return actual;
    }
    if(m_posPath.front() == actual.getBlockPos())
    {
        m_posPath.erase(m_posPath.begin());
        return next(actual);
    }
    return m_posPath.front();
}

bool Path::finished() const
{
    return m_posPath.empty() && m_roomPath.empty();
}

void Path::clear()
{
    m_posPath.clear();
    m_roomPath.clear();
}

void Path::pathToDoor(const Location & actual)
{
    if(m_roomPath.size() < 2)
    {
        if(actual.getRoom().lock() != m_dest.getRoom().lock())
        {
            clear();
            return;
        }
        m_posPath = PathFinder::path(actual.getRoom().lock(), actual.getBlockPos(), m_dest.getBlockPos());
        return;
    }

    std::shared_ptr<Room> r(actual.getRoom().lock());
    if(r != m_roomPath.front().lock())
    {
        clear();
        return;
    }
    m_roomPath.erase(m_roomPath.begin());
    std::shared_ptr<Room> rNext(m_roomPath.front().lock());

    const auto & doors(r->doors());
    auto it(std::find_if(doors.begin(), doors.end(), [rNext](const auto & d)
    {
        const std::shared_ptr<Room> rDoor(d.dest.getRoom().lock());
        return rDoor == rNext;
    }));
    if(it == doors.end())
    {
        clear();
        return;
    }
    m_posPath = PathFinder::path(r, actual.getBlockPos(), it->pos.getBlockPos());
    if(m_posPath.empty())
    {
        clear();
        return;
    }
}

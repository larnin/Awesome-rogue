#ifndef PATH_H
#define PATH_H

#include "Map/location.h"
#include <vector>
#include <memory>

class Room;

class Path
{
public:
    Path();
    Path(const Location & pos, const Location & dest);

    void newPath(const Location & pos, const Location & dest);
    Location next(const Location & actual);

    bool finished() const;
    Location dest() const;

private:
    void clear();
    void pathToDoor(const Location & actual);

    std::vector<sf::Vector2u> m_posPath;
    std::vector<std::weak_ptr<Room>> m_roomPath;
    Location m_dest;
    std::weak_ptr<Room> m_actualRoom;
};

#endif // PATH_H

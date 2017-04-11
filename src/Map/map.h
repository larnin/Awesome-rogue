#ifndef MAP_H
#define MAP_H

#include "Utilities/noncopiable.h"
#include "Events/eventreceiver.h"
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <memory>

template <typename T>
class EventItemLoaded;
class EventLoadFinished;

class Room;

class Map : public EventReceiver, private NonCopiable
{
public:
    Map();
    Map(Map &&) = default;
    Map & operator= (Map &&) = default;
    virtual ~Map();

    void addRoom(const std::shared_ptr<Room> & r);
    void removeRoom(const std::shared_ptr<Room> & r);
    unsigned int count() const;
    std::shared_ptr<Room> roomAt(const sf::Vector2i & pos);
    std::shared_ptr<Room> room(unsigned int id);
    std::vector<std::shared_ptr<Room>>::iterator begin();
    std::vector<std::shared_ptr<Room>>::iterator end();
    std::vector<std::shared_ptr<Room>>::const_iterator begin() const;
    std::vector<std::shared_ptr<Room>>::const_iterator end() const;

private:
    void onRoomLoaded(EventItemLoaded<Room> e);
    void onLoadFinished(EventLoadFinished);

    std::vector<std::shared_ptr<Room>> m_rooms;
};

#endif // MAP_H

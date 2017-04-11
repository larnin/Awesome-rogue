#ifndef LOCATION_H
#define LOCATION_H

#include <SFML/System/Vector2.hpp>
#include <memory>

class Room;

class Location
{
    friend class Map;

public:
    Location() = default;
    Location(const sf::Vector2u & blockPos, std::weak_ptr<Room> room = std::weak_ptr<Room>());
    Location(const sf::Vector2i &blockPos, std::weak_ptr<Room> room = std::weak_ptr<Room>());
    Location(const sf::Vector2f & pos, std::weak_ptr<Room> room = std::weak_ptr<Room>());
    Location(const sf::Vector2f & pos, unsigned int roomID);
    ~Location() = default;

    bool isValide() const; //retourne true si la salle existe, et si les coordonées sont dans la salle
    const std::weak_ptr<Room> & getRoom() const;
    unsigned int getRoomID() const;

    sf::Vector2f getPos() const;
    sf::Vector2u getBlockPos() const;
    sf::Vector2f toGlobalPos() const;
    sf::Vector2i toGlobalBlockPos() const;
    void move(sf::Vector2f dir);

    static Location fromGlobal(const sf::Vector2f & pos, std::weak_ptr<Room> room);
    static Location fromGlobalBlock(const sf::Vector2i & pos, std::weak_ptr<Room> room);
    static sf::Vector2i posToBlockPos(const sf::Vector2f & pos);

private:
    sf::Vector2f m_pos;
    std::weak_ptr<Room> m_room;
    unsigned int m_roomID;
};

#endif // LOCATION_H

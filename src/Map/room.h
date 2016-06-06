#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include "Utilities/matrix.h"
#include "Utilities/noncopiable.h"
#include "block.h"
#include "door.h"
#include "Generator/patern.h"

class Room : private NonCopiable
{
public:
    Room(const Patern & p, sf::Vector2i pos);
    Room(sf::Vector2u size, sf::Vector2i pos, Block def = Block());
    Room(Room &&) = default;
    Room & operator= (Room &&) = default;
    ~Room() = default;

    sf::Vector2i getPos() const;
    sf::Vector2u getSize() const;
    Block & operator()(const sf::Vector2u & pos);
    const Block & operator()(const sf::Vector2u & pos) const;

    Door doorAt(const sf::Vector2u & pos) const;
    const std::vector<Door> & doors() const;
    void setDoor(const Door & door);
    void delDoor(const sf::Vector2u & pos);
    unsigned int getID() const;
    bool isDiscovered() const;
    void uncover();

private:
    void drawDoor(sf::Vector2u pos);
    void removeDrawedDoor(sf::Vector2u pos);

    Matrix<Block> m_blocks;
    std::vector<Door> m_doors;
    sf::Vector2i m_pos;
    unsigned int m_UID;
    bool m_discovered;

    static unsigned int lastID;
};

#endif // ROOM_H
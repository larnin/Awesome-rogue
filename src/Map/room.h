#ifndef ROOM_H
#define ROOM_H

#include "Utilities/matrix.h"
#include "Utilities/noncopiable.h"
#include "block.h"
#include "door.h"
#include "Entities/entitytype.h"
#include "roomtype.h"
#include "File/serializable.h"
#include <vector>

class Patern;

class Room : public Serializable, private NonCopiable
{
    friend class Map;

public:
    Room(const json & j);
    Room(const Patern & p, sf::Vector2i pos, unsigned int id);
    Room(sf::Vector2u size, sf::Vector2i pos, unsigned int id, Block def = Block());
    Room(Room &&) = default;
    Room & operator= (Room &&) = default;
    ~Room() = default;

    sf::Vector2i getPos() const;
    sf::Vector2u getSize() const;
    //Block & operator()(const sf::Vector2u & pos);
    //const Block & operator()(const sf::Vector2u & pos) const;
    const Block & get(const sf::Vector2u & pos) const;
    Block & modify(const sf::Vector2u & pos);
    void set(const sf::Vector2u & pos, const Block & b);

    Door doorAt(const sf::Vector2u & pos) const;
    const std::vector<Door> & doors() const;
    void setDoor(const Door & door);
    void delDoor(const sf::Vector2u & pos);
    unsigned int getID() const;
    bool isDiscovered() const;
    void uncover();
    std::vector<EntityType> getAndResetPopulation();
    void setPopulation(std::vector<EntityType> entities);
    RoomType type() const;

    void closeDoors();
    void openDoors();

    std::string getRenderInfosName() const;
    bool modified;

protected:
    virtual json serialize() const override;

private:
    void drawDoor(sf::Vector2u pos);
    void removeDrawedDoor(sf::Vector2u pos);

    Matrix<Block> m_blocks;
    std::vector<Door> m_doors;
    sf::Vector2i m_pos;
    unsigned int m_UID;
    bool m_discovered;
    std::vector<EntityType> m_population;
    RoomType m_type;
    std::string m_renderInfosName;
};

#endif // ROOM_H

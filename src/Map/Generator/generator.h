#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>
#include <string>
#include <random>
#include <SFML/Graphics/Rect.hpp>
#include "Map/map.h"
#include "patern.h"
#include "Map/roomtype.h"

struct GenerationEnvironement
{
    GenerationEnvironement()
        : seed(0)
        , nbRooms(100)
        , nbTryDoors(2)
        , posBoss({0.2, 0.4, 0.6, 0.8})
        , nextFrom(0.9f)
        , minRoomSize(7, 7)
        , maxRoomSize(30, 30)
        , generatePaterns(false)
        , sideRarity(10)
        , populationDensity(1.0f)
    {

    }

    unsigned int seed;
    unsigned int nbRooms;
    unsigned int nbTryDoors;
    std::vector<float> posBoss;
    float nextFrom;
    sf::Vector2u minRoomSize;
    sf::Vector2u maxRoomSize;
    std::string paternsFileName;
    bool generatePaterns;
    unsigned int sideRarity;
    float populationDensity;
};

class Generator
{
public:
    Generator() = default;
    ~Generator() = default;
    Map generate(const GenerationEnvironement & env);

private:
    void loadPaterns(const std::string & filename, const sf::Vector2u & minSize, const sf::Vector2u & maxSize);
    void generatePaterns(RoomType type, const sf::Vector2u & minSize, const sf::Vector2u & maxSize);
    std::pair<Door, sf::IntRect> findDoorablePos(const Map & m, float doorPos, const sf::Vector2u & minSize, const sf::Vector2u & maxSize);
    sf::IntRect getMaxZone(const Map & m, const sf::Vector2i & startPos, const sf::Vector2u & minSize, const sf::Vector2u & maxSize);
    bool isValidDoorPos(const sf::Vector2u & pos, const std::shared_ptr<Room> & r);
    bool isValidDoorPos(const sf::Vector2u & pos, const Patern & p);
    Room createFirstRoom();
    const Patern & getPatern(const sf::Vector2u & maxSize, const sf::Vector2u & minSize, RoomType type);
    Room placeRoom(const Patern & p, sf::IntRect allowedRect, const Door & origineDoor, unsigned int sideRarity);
    void placeDoors(Map & m, unsigned int nbTry);
    Door isOkPlaceDoor(const std::shared_ptr<Room> & r, Map & m, sf::Vector2u pos, Orientation o);
    void Populate(Map & m, float density);

    std::vector<Patern> m_paterns;
    std::vector<Patern> m_paternsBoss;
    std::vector<Patern> m_paternsStart;
    std::default_random_engine m_engine;
};

#endif // GENERATOR_H

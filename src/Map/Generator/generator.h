#ifndef GENERATOR_H
#define GENERATOR_H

#include "patern.h"
#include "Map/roomtype.h"
#include "Map/orientation.h"
#include <SFML/Graphics/Rect.hpp>
#include <array>
#include <string>
#include <random>
#include <memory>

class Map;
class Door;
class Room;

struct GenerationEnvironement
{
    GenerationEnvironement()
        : seed(2)
        , nbRooms(100)
        , nbTryDoors(2)
        , posBoss({0.2, 0.4, 0.6, 0.8})
        , nextFrom(0.9f)
        , minRoomSize(7, 7)
        , maxRoomSize(30, 30)
        , sideRarity(10)
        , populationDensity(1.0f)
        , chestProbability(0.25f)
        , portalProbability(0.04f)
        , saveProbability(0.5f)
    {

    }

    unsigned int seed;
    unsigned int nbRooms;
    unsigned int nbTryDoors;
    std::array<float, 4> posBoss;
    float nextFrom;
    sf::Vector2u minRoomSize;
    sf::Vector2u maxRoomSize;
    std::string paternsFileName;
    unsigned int sideRarity;
    float populationDensity;
    float chestProbability;
    float portalProbability;
    float saveProbability;
};

class Generator
{
public:
    Generator() = default;
    ~Generator() = default;
    Map generate(const GenerationEnvironement & env);

private:
    void loadPaterns(const std::string & filename, const sf::Vector2u & minSize, const sf::Vector2u & maxSize);
    void generatePaterns(const sf::Vector2u & minSize, const sf::Vector2u & maxSize);
    std::pair<Door, sf::IntRect> findDoorablePos(const Map & m, float doorPos, const sf::Vector2u & minSize, const sf::Vector2u & maxSize);
    sf::IntRect getMaxZone(const Map & m, const sf::Vector2i & startPos, const sf::Vector2u & minSize, const sf::Vector2u & maxSize);
    bool isValidDoorPos(const sf::Vector2u & pos, const std::shared_ptr<Room> & r);
    bool isValidDoorPos(const sf::Vector2u & pos, const Patern & p);
    Room createFirstRoom();
    const Patern & getPatern(const sf::Vector2u & maxSize, const sf::Vector2u & minSize, RoomType type);
    Room placeRoom(const Patern & p, sf::IntRect allowedRect, const Door & origineDoor, unsigned int sideRarity, unsigned int id);
    void placeDoors(Map & m, unsigned int nbTry);
    Door isOkPlaceDoor(const std::shared_ptr<Room> & r, Map & m, sf::Vector2u pos, Orientation o);
    void placeChestsPortalsSave(Map & m, float chestProbability, float portalProbability, float saveProbability);
    void placeSolidWallBlockOn(std::shared_ptr<Room> & r, unsigned int id);
    void placeGroundBlockOn(std::shared_ptr<Room> & r, unsigned int id);
    void Populate(Map & m, float density);

    std::vector<Patern> m_paterns;
    std::default_random_engine m_engine;
};

#endif // GENERATOR_H

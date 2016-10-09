#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <sfml/System/Vector2.hpp>
#include <vector>
#include <memory>

class Room;

class PathFinder
{
public:
    static std::vector<std::weak_ptr<Room>> pathRooms(const std::weak_ptr<Room> & begin, const std::weak_ptr<Room> & end);
    static std::vector<sf::Vector2u> path(const std::weak_ptr<Room> & room, const sf::Vector2u & begin, const sf::Vector2u & end);
private:
    PathFinder() = delete;
    static std::vector<std::weak_ptr<Room>> pathOnRoomList(std::vector<std::pair<std::shared_ptr<Room>, std::shared_ptr<Room>>> & rooms);

    static float distance(const sf::Vector2u & pos1, const sf::Vector2u & pos2);
    static void addNext(const sf::Vector2u & pos, const sf::Vector2u & oldPos, unsigned int poid, std::vector<std::pair<sf::Vector2u, unsigned int>> & nextPos
                        , std::vector<std::pair<sf::Vector2u, sf::Vector2u>> &visitedPos);
    static std::vector<sf::Vector2u> pathOnPosList(const std::vector<std::pair<sf::Vector2u, sf::Vector2u>> &visitedPos);

    static const unsigned int straightDistance;
    static const unsigned int diagonalDistance;
};

#endif // PATHFINDER_H

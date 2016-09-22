#include <queue>
#include <algorithm>
#include <cassert>
#include "pathfinder.h"
#include "Map/room.h"

const unsigned int PathFinder::straightDistance(2);
const unsigned int PathFinder::diagonalDistance(3);

std::vector<std::weak_ptr<Room>> PathFinder::pathRooms(const std::weak_ptr<Room> & begin, const std::weak_ptr<Room> & end)
{
    std::shared_ptr<Room> rBegin(begin.lock()), rEnd(end.lock());
    if(!rBegin || !rEnd)
        return std::vector<std::weak_ptr<Room>>();

    if(rBegin == rEnd)
        return std::vector<std::weak_ptr<Room>>(1, begin);

    std::queue<std::shared_ptr<Room>> nextRooms;
    nextRooms.push(rBegin);
    std::vector<std::pair<std::shared_ptr<Room>, std::shared_ptr<Room>>> visitedRooms;

    while(!nextRooms.empty())
    {
        std::shared_ptr<Room> r(nextRooms.front());
        nextRooms.pop();
        for(const auto & d : r->doors())
        {
            if(!d.isValid())
                continue;
            std::shared_ptr<Room> rDest(d.dest.getRoom().lock());
            if(!rDest)
                continue;

            if(std::find_if(visitedRooms.begin(), visitedRooms.end(), [rDest](const auto & comp){return comp.second == rDest;}) != visitedRooms.end())
                continue;

            nextRooms.push(rDest);
            visitedRooms.emplace_back(r, rDest);

            if(rDest == rEnd)
                return pathOnRoomList(visitedRooms);
        }
    }

    return std::vector<std::weak_ptr<Room>>();
}

std::vector<std::weak_ptr<Room>> PathFinder::pathOnRoomList(std::vector<std::pair<std::shared_ptr<Room>, std::shared_ptr<Room>>> & rooms)
{
    std::shared_ptr<Room> begin(rooms.front().first);
    std::shared_ptr<Room> actual(rooms.back().second);

    std::vector<std::weak_ptr<Room>> returnList;
    returnList.push_back(actual);

    while(actual != begin)
    {
        auto it(std::find_if(rooms.begin(), rooms.end(), [actual](const auto & comp){return comp.second == actual;}));
        assert(it != rooms.end());
        actual = it->first;
        returnList.push_back(actual);
    }

    std::reverse(returnList.begin(), returnList.end());
    return returnList;
}

std::vector<sf::Vector2u> PathFinder::path(const std::weak_ptr<Room> & room, const sf::Vector2u & begin, const sf::Vector2u & end)
{
    std::shared_ptr<Room> r(room.lock());
    if(!r)
        return std::vector<sf::Vector2u>();

    if(begin.x > r->getSize().x || begin.y > r->getSize().y || end.x > r->getSize().x || end.y > r->getSize().y)
        return std::vector<sf::Vector2u>();

    if(getBoxType(r->get(begin).boxCaracts) != BoxType::EMPTY || getBoxType(r->get(end).boxCaracts) != BoxType::EMPTY)
        return std::vector<sf::Vector2u>();

    std::vector<std::pair<sf::Vector2u, unsigned int>> nextPos;
    nextPos.emplace_back(begin, 0);

    std::vector<std::pair<sf::Vector2u, sf::Vector2u>> visitedPos;

    while(!nextPos.empty())
    {
        auto it = std::min_element(nextPos.begin(), nextPos.end(),
                                   [end](const auto & a, const auto & b){return a.second + distance(a.first, end) < b.second + distance(b.first, end); });
        assert(it != nextPos.end());
        const sf::Vector2u pos(it->first);
        const unsigned int poid(it->second);
        nextPos.erase(it);

        Matrix<bool> localStates(sf::Vector2u(3, 3), false);
        for(unsigned int i(0) ; i < 3 ; i++)
            for(unsigned int j(0) ; j < 3 ; j++)
            {
                if(i == 1 && j == 1)
                    continue;
                sf::Vector2u localPos(pos.x+i-1, pos.y+j-1);
                if(localPos.x >= r->getSize().x || localPos.y >= r->getSize().y)
                    continue;
                localStates(sf::Vector2u(i, j)) = getBoxType(r->get(localPos).boxCaracts) == BoxType::EMPTY;
            }

        std::array<sf::Vector2i, 4> check({sf::Vector2i(0, -1), sf::Vector2i(-1, 0), sf::Vector2i(0, 1), sf::Vector2i(1, 0)});
        for(const auto p : check)
        {
            if(localStates(sf::Vector2u(p.x+1, p.y+1)))
            {
                sf::Vector2u newPos(pos.x+p.x, pos.y+p.y);
                addNext(newPos, pos, poid, nextPos, visitedPos);
                if(newPos == end)
                    return pathOnPosList(visitedPos);
            }
        }
        std::array<sf::Vector2i, 4> checkd({sf::Vector2i(-1, -1), sf::Vector2i(1, -1), sf::Vector2i(1, 1), sf::Vector2i(-1, 1)});
        for(const auto p : checkd)
        {
            if(localStates(sf::Vector2u(p.x+1, p.y+1)) && localStates(sf::Vector2u(1, p.y+1)) && localStates(sf::Vector2u(p.x+1, 1)))
            {
                sf::Vector2u newPos(pos.x+p.x, pos.y+p.y);
                addNext(newPos, pos, poid, nextPos, visitedPos);
                if(newPos == end)
                    return pathOnPosList(visitedPos);
            }
        }
    }

    return std::vector<sf::Vector2u>();
}

void PathFinder::addNext(const sf::Vector2u & pos, const sf::Vector2u & oldPos, unsigned int poid, std::vector<std::pair<sf::Vector2u, unsigned int>> & nextPos
                       , std::vector<std::pair<sf::Vector2u, sf::Vector2u>> &visitedPos)
{
    if(std::find_if(visitedPos.begin(), visitedPos.end(), [pos](const auto & p){return p.second == pos;}) != visitedPos.end())
        return;
    visitedPos.emplace_back(oldPos, pos);
    nextPos.emplace_back(pos, poid);
}

float PathFinder::distance(const sf::Vector2u & pos1, const sf::Vector2u & pos2)
{
    sf::Vector2u v(std::abs(int(pos1.x)-int(pos2.x)), std::abs(int(pos1.y)-int(pos2.y)));
    unsigned int diagonalDist(std::min(v.x, v.y));
    return diagonalDist*PathFinder::diagonalDistance + (v.x + v.y - 2*diagonalDist)*PathFinder::straightDistance;
}

std::vector<sf::Vector2u> PathFinder::pathOnPosList(const std::vector<std::pair<sf::Vector2u, sf::Vector2u>> &visitedPos)
{
    sf::Vector2u begin(visitedPos.front().first);
    sf::Vector2u actual(visitedPos.back().second);

    std::vector<sf::Vector2u> returnList;
    returnList.push_back(actual);

    while(actual != begin)
    {
        auto it(std::find_if(visitedPos.begin(), visitedPos.end(), [actual](const auto & comp){return comp.second == actual;}));
        assert(it != visitedPos.end());
        actual = it->first;
        returnList.push_back(actual);
    }

    std::reverse(returnList.begin(), returnList.end());
    return returnList;
}


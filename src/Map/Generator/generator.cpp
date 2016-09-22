#include <memory>
#include <stdexcept>
#include <algorithm>
#include "generator.h"
#include "Map/orientation.h"
#include "Entities/entitytype.h"
#include "Collisions/pathfinder.h"
#include "Map/map.h"
#include "Map/door.h"
#include "Map/room.h"

const unsigned int multiplierStartRoom(2);
const unsigned int multiplierBossRoom(2);

Map Generator::generate(const GenerationEnvironement & env)
{
    if(!env.paternsFileName.empty())
        loadPaterns(env.paternsFileName, env.minRoomSize, env.maxRoomSize);
    generatePaterns(env.minRoomSize, env.maxRoomSize);

    m_engine.seed(env.seed);

    Map m;
    m.addRoom(std::make_shared<Room>(createFirstRoom()));

    int retry(0);
    while(m.count() < env.nbRooms)
    {
        try
        {
            unsigned int bossRoom(0);
            for(unsigned int i(0) ; i < env.posBoss.size() ; i++)
            {
                if(m.count() >= env.posBoss[i]*env.nbRooms && m.count()-1 < env.posBoss[i]*env.nbRooms)
                {
                    bossRoom = i+1;
                    break;
                }

            }

            float nextFrom(env.nextFrom + std::uniform_real_distribution<float>(-retry/100.0f, retry/100.0f)(m_engine));
            auto door(bossRoom > 0 ? findDoorablePos(m, env.nextFrom, env.minRoomSize*multiplierBossRoom, env.maxRoomSize*multiplierBossRoom)
                        : findDoorablePos(m, nextFrom, env.minRoomSize, env.maxRoomSize));

            sf::Vector2u maxRect(std::min(int(env.maxRoomSize.x * (bossRoom>0?multiplierBossRoom:1)), door.second.width)
                                 , std::min(int(env.maxRoomSize.y * (bossRoom>0?multiplierBossRoom:1)), door.second.height));

            std::shared_ptr<Room> r(std::make_shared<Room>(placeRoom(getPatern(maxRect, env.minRoomSize
                                        , bossRoom == 0 ? RoomType::NORMAL_ROOM
                                        : bossRoom == 1 ? RoomType::BOSS1_ROOM
                                        : bossRoom == 2 ? RoomType::BOSS2_ROOM
                                        : bossRoom == 3 ? RoomType::BOSS3_ROOM
                                                        : RoomType::BOSS4_ROOM)
                                        , door.second, door.first, env.sideRarity, m.count())));
            std::shared_ptr<Room> initialRoom(door.first.pos.getRoom().lock());
            assert(initialRoom);

            m.addRoom(r);

            Orientation doorOrientation(door.first.getOrientation());
            sf::Vector2i doorGlobalPos(door.first.pos.toGlobalBlockPos());
            door.first.dest = Location::fromGlobalBlock(relative(doorGlobalPos, doorOrientation, 2), r);
            Door otherSideDoor(Location::fromGlobalBlock(relative(doorGlobalPos, doorOrientation, 1), r)
                               , Location::fromGlobalBlock(relative(doorGlobalPos, doorOrientation, -1), initialRoom));
            r->setDoor(otherSideDoor);
            initialRoom->setDoor(door.first);

            retry = 0;
        }
        catch(const std::exception &)
        {
            retry ++;
        }
    }

    placeDoors(m, env.nbTryDoors);

    placeChestsPortalsSave(m, env.chestProbability, env.portalProbability, env.saveProbability);

    Populate(m, env.populationDensity);

    return m;
}

void Generator::loadPaterns(const std::string & filename, const sf::Vector2u & minSize, const sf::Vector2u & maxSize)
{
    m_paterns.clear();

    auto paterns = Patern::load(filename);
    for(const auto & p : paterns)
    {
        if(p.type != RoomType::NORMAL_ROOM)
        {
            m_paterns.push_back(p);
            continue;
        }
        sf::Vector2u size(p.getSize());
        if((size.x >= minSize.x && size.x <= maxSize.x && size.y >= minSize.y && size.y <= maxSize.y)
                || (size.x >= minSize.y && size.x <= maxSize.y && size.y >= minSize.x && size.y <= maxSize.x))
        {
            m_paterns.push_back(p);
            continue;
        }
    }
}

void Generator::generatePaterns(const sf::Vector2u & minSize, const sf::Vector2u & maxSize)
{
    RoomType type(RoomType::NORMAL_ROOM);
    auto lambdaFunct([&type](unsigned int count, const Patern & p){if(p.type == type) return count+1; else return count;});

    m_engine.seed(0);

    std::uniform_int_distribution<unsigned int> xDistrib(minSize.x, maxSize.x);
    std::uniform_int_distribution<unsigned int> yDistrib(minSize.y, maxSize.y);
    auto lambdaCreate([](RoomType type, unsigned int rarity, const sf::Vector2u & size)
    {
        Patern p(size);
        p.type = type;
        p.rarity = rarity;
        return p;
    });

    int nb(30-std::accumulate(m_paterns.begin(), m_paterns.end(), 0u, lambdaFunct));
    while(nb-->0)
        m_paterns.push_back(lambdaCreate(type, 50, sf::Vector2u(xDistrib(m_engine), yDistrib(m_engine))));

    type = RoomType::START_ROOM;
    if(std::accumulate(m_paterns.begin(), m_paterns.end(), 0u, lambdaFunct) == 0)
        m_paterns.push_back(lambdaCreate(type, 50, maxSize));

    type = RoomType::BOSS1_ROOM;
    if(std::accumulate(m_paterns.begin(), m_paterns.end(), 0u, lambdaFunct) == 0)
        m_paterns.push_back(lambdaCreate(type, 50, maxSize));

    type = RoomType::BOSS2_ROOM;
    if(std::accumulate(m_paterns.begin(), m_paterns.end(), 0u, lambdaFunct) == 0)
        m_paterns.push_back(lambdaCreate(type, 50, maxSize));

    type = RoomType::BOSS3_ROOM;
    if(std::accumulate(m_paterns.begin(), m_paterns.end(), 0u, lambdaFunct) == 0)
        m_paterns.push_back(lambdaCreate(type, 50, maxSize));

    type = RoomType::BOSS4_ROOM;
    if(std::accumulate(m_paterns.begin(), m_paterns.end(), 0u, lambdaFunct) == 0)
        m_paterns.push_back(lambdaCreate(type, 50, maxSize));

    type = RoomType::FINAL_ROOM;
    if(std::accumulate(m_paterns.begin(), m_paterns.end(), 0u, lambdaFunct) == 0)
        m_paterns.push_back(lambdaCreate(type, 50, maxSize));

}

Room Generator::createFirstRoom()
{
    unsigned int maxSize(1000);
    getPatern(sf::Vector2u(maxSize, maxSize), sf::Vector2u(0, 0), RoomType::START_ROOM);
    return Room(getPatern(sf::Vector2u(maxSize, maxSize), sf::Vector2u(0, 0), RoomType::START_ROOM), sf::Vector2i(0, 0), 0);
}

std::pair<Door, sf::IntRect> Generator::findDoorablePos(const Map & m, float doorPos, const sf::Vector2u & minSize, const sf::Vector2u & maxSize)
{
    doorPos = std::min(std::max(0.0f, doorPos), 1.0f);
    unsigned int startId(m.count()*doorPos);
    if(startId >= m.count())
        startId = m.count()-1;


    for(int delta(0) ; delta < int(m.count()) ; delta++)
    {
        for(int side(-1) ; side <= 1 ; side +=2)
        {
            int id(startId + delta*side);
            if(id < 0 || id >= int(m.count()))
                continue;

            const auto &r(*(m.begin()+id));
            if(!r)
                continue;
            std::uniform_int_distribution<int> distribX(1, r->getSize().x-2);
            std::uniform_int_distribution<int> distribY(1, r->getSize().y-2);
            //sf::IntRect rect(r->getPos(), sf::Vector2i(r->getSize()));

            std::vector<Orientation> sides{Orientation::UP, Orientation::LEFT, Orientation::RIGHT, Orientation::DOWN};
            std::shuffle(sides.begin(), sides.end(), m_engine);

            for(Orientation o : sides)
            {
                sf::Vector2u pos(relativeOn(sf::Vector2u(distribX(m_engine), distribY(m_engine)), o, sf::Rect<unsigned int>(sf::Vector2u(0, 0), r->getSize()-sf::Vector2u(1, 1))));
                if(!isValidDoorPos(pos, r))
                    continue;
                sf::Vector2i startPos(sf::Vector2i(pos) + r->getPos() + orientationToVector<int>(o));
                sf::IntRect rect(getMaxZone(m, startPos, minSize, maxSize));
                if(rect.width < int(minSize.x) || rect.height < int(minSize.y))
                    continue;

                return std::pair<Door,sf::IntRect>(Door(Location(pos, r)),rect);
            }
        }
    }
    throw std::runtime_error("Impossible to create a valid room");
}

sf::IntRect bestRegionOf(std::vector<sf::IntRect> regions)
{
    sf::IntRect bestRegion;
    int bestSurface(0);

    for(const auto & region : regions)
    {
        if(region.width<=0 || region.height<=0)
            continue;

        int surface = region.width*region.height;
        if(surface > bestSurface)
        {
            bestRegion = region;
            bestSurface = surface;
        }
    }
    return bestRegion;
}

sf::IntRect Generator::getMaxZone(const Map & m, const sf::Vector2i & startPos, const sf::Vector2u & minSize, const sf::Vector2u & maxSize)
{
    sf::IntRect emptyZone(startPos - sf::Vector2i(maxSize), sf::Vector2i(maxSize)*2+sf::Vector2i(1, 1));

    for(const auto & r : m)
    {
        sf::IntRect roomZone(r->getPos(), sf::Vector2i(r->getSize()));
        if(!emptyZone.intersects(roomZone))
            continue;

        std::vector<sf::IntRect> possibleRegions;
        possibleRegions.emplace_back(emptyZone.left, emptyZone.top, roomZone.left-emptyZone.left, emptyZone.height);
        possibleRegions.emplace_back(emptyZone.left, roomZone.top+roomZone.height, emptyZone.width, emptyZone.height-(roomZone.top+roomZone.height-emptyZone.top));
        possibleRegions.emplace_back(emptyZone.left, emptyZone.top, emptyZone.width, roomZone.top-emptyZone.top);
        possibleRegions.emplace_back(roomZone.left+roomZone.width, emptyZone.top, emptyZone.width-(roomZone.left+roomZone.width-emptyZone.left), emptyZone.height);

        emptyZone = bestRegionOf(possibleRegions);
        if(emptyZone.width < int(minSize.x) || emptyZone.height < int(minSize.y))
            return emptyZone;
        if(!emptyZone.contains(startPos))
            return sf::IntRect();
    }
    return emptyZone;
}

bool Generator::isValidDoorPos(const sf::Vector2u & pos, const std::shared_ptr<Room> & r)
{
    if(!r)
        return false;

    if(pos.x > r->getSize().x || pos.y > r->getSize().y)
        return false;

    if(pos.x > 0 && pos.y > 0 && pos.x < r->getSize().x-1 && pos.y < r->getSize().y-1)
        return false;

    if(pos == sf::Vector2u(0, 0) || pos == sf::Vector2u(0, r->getSize().y-1)
            || pos == sf::Vector2u(r->getSize().x-1, 0) || pos == sf::Vector2u(r->getSize().x-1, r->getSize().y-1))
        return false;

    const std::vector<unsigned int> validWallIds{2, 3};
    bool validBlock(std::find(validWallIds.begin(), validWallIds.end(), r->get(pos).wallID) != validWallIds.end());
    if(!validBlock)
        return false;

    Orientation o(orientedSide(pos, sf::Rect<unsigned int>(sf::Vector2u(0, 0), r->getSize())));

    return getBoxType(r->get(sf::Vector2u(relative(sf::Vector2i(pos), o, -1))).boxCaracts) == BoxType::EMPTY;
}

bool Generator::isValidDoorPos(const sf::Vector2u & pos, const Patern & p)
{
    if(pos.x > p.getSize().x || pos.y > p.getSize().y)
        return false;

    if(pos.x > 0 && pos.y > 0 && pos.x < p.getSize().x-1 && pos.y < p.getSize().y-1)
        return false;

    const std::vector<unsigned int> validWallIds{2, 3};
    bool validBlock(std::find(validWallIds.begin(), validWallIds.end(), p(pos).wallID) != validWallIds.end());
    if(!validBlock)
        return false;

    Orientation o(orientedSide(pos, sf::Rect<unsigned int>(sf::Vector2u(0, 0), p.getSize())));

    return getBoxType(p(sf::Vector2u(relative(sf::Vector2i(pos), o, -1))).boxCaracts) == BoxType::EMPTY;
}

const Patern & Generator::getPatern(const sf::Vector2u & maxSize, const sf::Vector2u & minSize, RoomType type)
{
    std::vector<unsigned int> poids;
    std::vector<unsigned int> indexs;
    for(unsigned int i(0) ; i < m_paterns.size() ; i++)
    {
        Patern & p(m_paterns[i]);
        if(p.type != type)
            continue;
        if(p.getSize().x < minSize.x || p.getSize().y < minSize.y || p.getSize().x > maxSize.x || p.getSize().y > maxSize.y)
            continue;
        poids.push_back(p.rarity);
        indexs.push_back(i);
    }

    if(indexs.size() == 0)
        assert(false);

    if(indexs.size() == 1)
        return m_paterns[indexs[0]];

    std::discrete_distribution<int> distrib(poids.begin(), poids.end());
    return m_paterns[indexs[distrib(m_engine)]];
}

Room Generator::placeRoom(const Patern & p, sf::IntRect allowedRect, const Door & origineDoor, unsigned int sideRarity, unsigned int id)
{
    Orientation doorOrientation(origineDoor.getOrientation());
    Orientation moveOrientation(doorOrientation == Orientation::UP || doorOrientation == Orientation::DOWN ? Orientation::RIGHT : Orientation::DOWN);
    sf::Vector2i globalDestDoor(relative(origineDoor.pos.toGlobalBlockPos(), doorOrientation, 1));

    sf::Vector2i minPosition(globalDestDoor);
    switch(doorOrientation)
    {
    case Orientation::UP:
        minPosition -= sf::Vector2i(p.getSize())-sf::Vector2i(1, 1);
    break;
    case Orientation::DOWN:
        minPosition -= sf::Vector2i(p.getSize().x-1, 0);
    break;
    case Orientation::LEFT:
        minPosition -= sf::Vector2i(p.getSize())-sf::Vector2i(1, 1);
    break;
    case Orientation::RIGHT:
        minPosition -= sf::Vector2i(0, p.getSize().y-1);
    break;
    default:
        assert(false);
    break;
    }

    sf::Vector2i moveDirection(orientationToVector<int>(moveOrientation));
    moveDirection = sf::Vector2i(std::abs(moveDirection.x), std::abs(moveDirection.y));

    minPosition = sf::Vector2i(std::max(minPosition.x, allowedRect.left), std::max(minPosition.y, allowedRect.top));

    sf::Vector2i maxPosition(minPosition + sf::Vector2i(p.getSize().x*std::abs(moveDirection.x), p.getSize().y*std::abs(moveDirection.y)));
    maxPosition = sf::Vector2i(std::min(maxPosition.x, allowedRect.left+allowedRect.width-int(p.getSize().x))
                               , std::min(maxPosition.y, allowedRect.top+allowedRect.height-int(p.getSize().y)));

    unsigned int maxI((maxPosition.x-minPosition.x)*moveDirection.x + (maxPosition.y-minPosition.y)*moveDirection.y);
    std::vector<sf::Vector2i> allowedPos;
    std::vector<unsigned int> poids;

    for(int i(0) ; i < int(maxI) ; i++)
    {
        sf::Vector2i potentialRoomPos(minPosition+i*moveDirection);
        sf::Vector2u relativeDoorPos(globalDestDoor-potentialRoomPos);
        if(isValidDoorPos(relativeDoorPos, p))
        {
            unsigned int poid(1);
            if((moveDirection.x != 0 && (potentialRoomPos.x == allowedRect.left || potentialRoomPos.x + int(p.getSize().x) == allowedRect.left+allowedRect.width))
               || (moveDirection.y != 0 && (potentialRoomPos.y == allowedRect.top || potentialRoomPos.y + int(p.getSize().y) == allowedRect.top+allowedRect.height)))
                poid = sideRarity;
            poids.push_back(poid);
            allowedPos.push_back(potentialRoomPos);
        }
    }

    if(allowedPos.size() == 0)
        throw std::runtime_error("No allowed pos !");

    std::discrete_distribution<int> distrib(poids.begin(), poids.end());
    return Room(p, allowedPos[distrib(m_engine)], id);
}

void Generator::placeDoors(Map & m, unsigned int nbTry)
{
    if(nbTry == 0)
        return;

    for(auto & r : m)
    {
        if(!r)
            continue;

        std::vector<Door> possibleDoors;
        for(unsigned int i(1) ; i < r->getSize().x-1 ; i++)
        {
            Door d(isOkPlaceDoor(r, m, sf::Vector2u(i, 0), Orientation::UP));
            if(d.isValid())
                possibleDoors.push_back(d);
            d = isOkPlaceDoor(r, m, sf::Vector2u(i, r->getSize().y-1), Orientation::DOWN);
            if(d.isValid())
                possibleDoors.push_back(d);
        }
        for(unsigned int i(1) ; i < r->getSize().y-1 ; i++)
        {
            Door d(isOkPlaceDoor(r, m, sf::Vector2u(0, i), Orientation::LEFT));
            if(d.isValid())
                possibleDoors.push_back(d);
            d = isOkPlaceDoor(r, m, sf::Vector2u(r->getSize().x-1, i), Orientation::RIGHT);
            if(d.isValid())
                possibleDoors.push_back(d);
        }

        for(unsigned int i(0) ; i < nbTry ; i++)
        {
            if(possibleDoors.size() == 0)
                break;

            std::uniform_int_distribution<unsigned int> distrib(0, possibleDoors.size()-1);
            Door d(possibleDoors[distrib(m_engine)]);
            auto it(std::find_if(r->doors().begin(), r->doors().end(), [d](const auto & door){return d.dest.getRoom().lock() == door.dest.getRoom().lock();}));
            if(it != r->doors().end())
                continue;

            r->setDoor(d);
            Orientation o(d.getOrientation());
            Door otherDoor(Location(relative(sf::Vector2i(d.dest.getBlockPos()), o, -1), d.dest.getRoom())
                           , Location(relative(sf::Vector2i(d.pos.getBlockPos()), o, -1), d.pos.getRoom()));
            d.dest.getRoom().lock()->setDoor(otherDoor);
        }
    }
}

Door Generator::isOkPlaceDoor(const std::shared_ptr<Room> & r, Map & m, sf::Vector2u pos, Orientation o)
{
    if(!isValidDoorPos(pos, r))
        return Door();
    Door d(Location(pos, r));
    sf::Vector2i destPos(relative(d.pos.toGlobalBlockPos(), o, 1));
    auto rDest(m.roomAt(destPos));
    if(!rDest)
        return Door();
    if(rDest == r)
        assert(false);
    d.dest = Location::fromGlobalBlock(relative(destPos, o, 1), rDest);
    if(!isValidDoorPos(sf::Vector2u(relative(sf::Vector2i(d.dest.getBlockPos()), o, -1)), rDest))
        return Door();
    return d;
}

void Generator::placeChestsPortalsSave(Map & m, float chestProbability, float portalProbability, float saveProbability)
{
    const unsigned int chestID(6);
    const unsigned int portalID(8);
    const unsigned int saveID(9);

    std::uniform_real_distribution<float> d(0, 1);

    for(std::shared_ptr<Room> & r : m)
    {
        if(r->type() != NORMAL_ROOM)
            continue;

        if(d(m_engine) <= chestProbability)
            placeSolidWallBlockOn(r, chestID);
        if(d(m_engine) <= portalProbability)
            placeGroundBlockOn(r, portalID);
        if(d(m_engine) <= saveProbability)
            placeGroundBlockOn(r, saveID);
    }
}

void Generator::placeSolidWallBlockOn(std::shared_ptr<Room> & r, unsigned int id)
{
    if(!r)
        return;

    std::uniform_int_distribution<unsigned int> dX(1, r->getSize().x-2);
    std::uniform_int_distribution<unsigned int> dY(1, r->getSize().y-2);

    for(unsigned int i(0) ; i < 10 ; i++)
    {
        sf::Vector2u pos(dX(m_engine), dY(m_engine));
        Block & b(r->modify(pos));
        Block old(b);
        if(getBoxType(b.boxCaracts) != BoxType::EMPTY)
            continue;

        b.wallID = id;
        b.wallOrientation = createOrientation(Rotation::ROT_0, false, false);
        b.boxCaracts = createBoxCaracts(Rotation::ROT_0, false, false, BoxType::FULL);

        bool canPass(true);
        for(int a(-1) ; a <= 1 ; a++)
            for(int b(-1) ; b <= 1 ; b++)
                for(int c(-1) ; c <= 1 ; c++)
                    for(int d(-1) ; d <= 1 ; d++)
                    {
                        if(!canPass)
                            break;
                        if(a == c && b == d)
                            continue;
                        if((a == 0 && b == 0) || (c == 0 && d == 0))
                            continue;
                        sf::Vector2u startPos(pos.x+a, pos.y+b);
                        sf::Vector2u endPos(pos.x+c, pos.y+d);
                        if(PathFinder::path(r, startPos, endPos).size() <= 1)
                            canPass = false;
                    }
        if(!canPass)
        {
            b = old;
            continue;
        }

        break;
    }
}

void Generator::placeGroundBlockOn(std::shared_ptr<Room> & r, unsigned int id)
{
    if(!r)
        return;

    std::uniform_int_distribution<unsigned int> dX(1, r->getSize().x-2);
    std::uniform_int_distribution<unsigned int> dY(1, r->getSize().y-2);

    for(unsigned int i(0) ; i < 10 ; i++)
    {
        sf::Vector2u pos(dX(m_engine), dY(m_engine));
        Block & b(r->modify(pos));
        if(getBoxType(b.boxCaracts) != BoxType::EMPTY)
            continue;

        b.wallID = id;
        b.wallOrientation = createOrientation(Rotation::ROT_0, false, false);

        break;
    }
}

void Generator::Populate(Map & m, float density)
{
    for(auto & r : m)
    {
        if(!r)
            continue;
        if(r->type() != RoomType::NORMAL_ROOM)
            continue;

        const float divisor(0.02f);

        std::vector<float> proba(populationProbability(float(r->getID())/m.count()));
        std::discrete_distribution<unsigned int> dType(proba.begin(), proba.end());

        int maxMob(std::pow(r->getSize().x*r->getSize().y*divisor*density, 1.5f));
        int nbMobs(std::uniform_int_distribution<unsigned int>(0, maxMob)(m_engine));
        std::vector<EntityType> types(nbMobs, EntityType::E_MAX);
        for(auto & t : types)
            t = EntityType(dType(m_engine));
        r->setPopulation(types);
    }
}

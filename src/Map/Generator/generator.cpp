#include <memory>
#include <stdexcept>
#include <algorithm>
#include "generator.h"
#include "Map/orientation.h"
#include "Entities/entitytype.h"

const unsigned int multiplierStartRoom(2);
const unsigned int multiplierBossRoom(2);

Map Generator::generate(const GenerationEnvironement & env)
{
    if(env.generatePaterns)
    {
        generatePaterns(RoomType::NORMAL_ROOM, env.minRoomSize, env.maxRoomSize);
        generatePaterns(RoomType::BOSS_ROOM, env.minRoomSize*multiplierBossRoom, env.maxRoomSize*multiplierBossRoom);
        generatePaterns(RoomType::START_ROOM, env.minRoomSize*multiplierStartRoom, env.maxRoomSize*multiplierStartRoom);
    }
    else loadPaterns(env.paternsFileName, env.minRoomSize, env.maxRoomSize);

    m_engine.seed(env.seed);

    Map m;
    m.addRoom(std::make_shared<Room>(createFirstRoom()));

    int retry(0);
    while(m.count() < env.nbRooms)
    {
        try
        {
            bool bossRoom(false);
            for(float posBoss : env.posBoss)
            {
                if(m.count() >= posBoss*env.nbRooms && m.count()-1 < posBoss*env.nbRooms)
                {
                    bossRoom = true;
                    break;
                }
            }

            float nextFrom(env.nextFrom + std::uniform_real_distribution<float>(-retry/100.0f, retry/100.0f)(m_engine));
            auto door(bossRoom ? findDoorablePos(m, env.nextFrom, env.minRoomSize*multiplierBossRoom, env.maxRoomSize*multiplierBossRoom)
                        : findDoorablePos(m, nextFrom, env.minRoomSize, env.maxRoomSize));

            sf::Vector2u maxRect(std::min(int(env.maxRoomSize.x), door.second.width), std::min(int(env.maxRoomSize.y), door.second.height));
            if(bossRoom)
                maxRect *= 2u;


            std::shared_ptr<Room> r(std::make_shared<Room>(placeRoom(
                                        getPatern(maxRect, env.minRoomSize, bossRoom ? RoomType::BOSS_ROOM : RoomType::NORMAL_ROOM)
                                        , door.second, door.first, env.sideRarity)));
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

    Populate(m, env.populationDensity);

    return m;
}

void Generator::loadPaterns(const std::string & filename, const sf::Vector2u & minSize, const sf::Vector2u & maxSize)
{
    m_paterns.clear();
    m_paternsBoss.clear();
    m_paternsStart.clear();

    auto paterns = Patern::load(filename);
    for(const auto & p : paterns)
    {
        if(p.type == RoomType::START_ROOM)
        {
            m_paternsStart.push_back(p);
            continue;
        }
        if(p.type == RoomType::BOSS_ROOM)
        {
            m_paternsBoss.push_back(p);
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

    if(m_paternsStart.empty())
        generatePaterns(RoomType::START_ROOM, sf::Vector2u(20, 20), sf::Vector2u(20, 20));
    if(m_paternsBoss.empty())
        generatePaterns(RoomType::BOSS_ROOM, minSize*2u, maxSize*2u);
    if(m_paterns.empty())
        generatePaterns(RoomType::NORMAL_ROOM, minSize, maxSize);
}

void Generator::generatePaterns(RoomType type, const sf::Vector2u & minSize, const sf::Vector2u & maxSize)
{
    std::vector<Patern> & paterns(type == RoomType::START_ROOM ? m_paternsStart
                              :   type == RoomType::BOSS_ROOM ? m_paternsBoss
                                                                    : m_paterns);
    paterns.clear();
    unsigned int nbRooms(type == RoomType::NORMAL_ROOM ? 20
                       : type == RoomType::BOSS_ROOM ? 4
                                                              : 1);

    m_engine.seed(0);
    std::uniform_int_distribution<unsigned int> xDistrib(minSize.x, maxSize.x);
    std::uniform_int_distribution<unsigned int> yDistrib(minSize.y, maxSize.y);

    while(paterns.size() < nbRooms)
    {
        Patern p(sf::Vector2u(xDistrib(m_engine), yDistrib(m_engine)));
        p.type = type;
        p.rarity = 50;
        paterns.push_back(p);
    }

    Patern p(minSize);
    p.rarity = 1;
    paterns.push_back(p);
}

Room Generator::createFirstRoom()
{
    std::uniform_int_distribution<unsigned int> RoomIDDistrib(0, m_paternsStart.size()-1);
    return Room(m_paternsStart[RoomIDDistrib(m_engine)], sf::Vector2i(0, 0));
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
    bool validBlock(std::find(validWallIds.begin(), validWallIds.end(), ((*r)(pos)).wallID) != validWallIds.end());
    if(!validBlock)
        return false;

    Orientation o(orientedSide(pos, sf::Rect<unsigned int>(sf::Vector2u(0, 0), r->getSize())));

    return getBoxType(((*r)(sf::Vector2u(relative(sf::Vector2i(pos), o, -1)))).boxCaracts) == BoxType::EMPTY;
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
    std::vector<Patern> & paterns(type == RoomType::START_ROOM ? m_paternsStart
                              :   type == RoomType::BOSS_ROOM ? m_paternsBoss
                                                                    : m_paterns);
    std::vector<unsigned int> poids;
    std::vector<unsigned int> indexs;
    for(unsigned int i(0) ; i < paterns.size() ; i++)
    {
        Patern & p(paterns[i]);
        if(p.getSize().x < minSize.x || p.getSize().y < minSize.y || p.getSize().x > maxSize.x || p.getSize().y > maxSize.y)
            continue;
        poids.push_back(p.rarity);
        indexs.push_back(i);
    }

    if(indexs.size() == 0)
        assert(false);

    if(indexs.size() == 1)
        return paterns[indexs[0]];

    std::discrete_distribution<int> distrib(poids.begin(), poids.end());
    return paterns[indexs[distrib(m_engine)]];
}

Room Generator::placeRoom(const Patern & p, sf::IntRect allowedRect, const Door & origineDoor, unsigned int sideRarity)
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
    return Room(p, allowedPos[distrib(m_engine)]);
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

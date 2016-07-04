#include "room.h"

unsigned int Room::lastID(0);

Room::Room(const Patern & p, sf::Vector2i pos)
    : m_blocks(p.getSize())
    , m_pos(pos)
    , m_UID(lastID++)
    , m_discovered(false)
    , m_type(p.type)
{
    for(unsigned int i(0) ; i < p.getSize().x ; i++)
        for(unsigned int j(0) ; j < p.getSize().y ; j++)
            m_blocks(sf::Vector2u(i, j)) = p(sf::Vector2u(i, j));
}

Room::Room(sf::Vector2u size, sf::Vector2i pos, Block def)
    : m_blocks(size, def)
    , m_pos(pos)
    , m_UID(lastID++)
    , m_discovered(false)
    , m_type(NORMAL_ROOM)
{

}

sf::Vector2i Room::getPos() const
{
    return m_pos;
}

sf::Vector2u Room::getSize() const
{
    return m_blocks.getSize();
}

Block & Room::operator()(const sf::Vector2u & pos)
{
    return m_blocks(pos);
}

const Block & Room::operator()(const sf::Vector2u & pos) const
{
    return m_blocks(pos);
}

Door Room::doorAt(const sf::Vector2u & pos) const
{
    auto it(std::find_if(m_doors.begin(), m_doors.end(), [pos](auto door){return door.pos.getBlockPos() == pos;}));
    if(it == m_doors.end())
        return Door();
    return *it;
}

const std::vector<Door> & Room::doors() const
{
    return m_doors;
}

void Room::setDoor(const Door & door)
{
    std::shared_ptr<Room> rLock(door.pos.getRoom());
    if(rLock.get() != this)
        return;
    if(doorAt(door.pos.getBlockPos()).pos.isValide())
        return;
    m_doors.push_back(door);
    drawDoor(door.pos.getBlockPos());
}

void Room::delDoor(const sf::Vector2u & pos)
{
    auto it(std::find_if(m_doors.begin(), m_doors.end(), [pos](auto door){return door.pos.getBlockPos() == pos;}));
    if(it == m_doors.end())
        return;

    std::swap(*it, m_doors.back());
    m_doors.pop_back();
    removeDrawedDoor(pos);
}

unsigned int Room::getID() const
{
    return m_UID;
}

bool Room::isDiscovered() const
{
    return m_discovered;
}

void Room::uncover()
{
    m_discovered = true;

    const unsigned int uncoveredDoorID(4);

    for(const Door & d : m_doors)
    {
        std::shared_ptr<Room> r(d.dest.getRoom().lock());
        if(!r)
            continue;
        (*r)(sf::Vector2u(relative(sf::Vector2i(d.dest.getBlockPos()), d.getOrientation(), -1))).groundID = uncoveredDoorID;
    }
}

RoomType Room::type() const
{
    return m_type;
}

std::vector<EntityType> Room::getAndResetPopulation()
{
    auto e(m_population);
    m_population.clear();
    return e;
}

void Room::setPopulation(std::vector<EntityType> entities)
{
    m_population = entities;
}

void Room::drawDoor(sf::Vector2u pos)
{
    assert(!(pos == sf::Vector2u(0, 0) || pos == sf::Vector2u(0, getSize().y-1)
             || pos == sf::Vector2u(getSize().x-1, 0) || pos == sf::Vector2u(getSize().x-1, getSize().y-1)));

    const unsigned int doorID(5);
    Orientation o(orientedSide(pos, sf::Rect<unsigned int>(sf::Vector2u(0, 0), getSize())));
    Rotation rot(o == Orientation::UP ? Rotation::ROT_0
               : o == Orientation::RIGHT ? Rotation::ROT_90
               : o == Orientation::DOWN ? Rotation::ROT_180
                                        : Rotation::ROT_270);
    m_blocks(pos) = Block(doorID, createOrientation(rot, false, false));

    const unsigned int wallCornerID(1);
    const unsigned int wallLineID(2);
    const unsigned int wallLineNoflipID(3);

    Orientation oSide(static_cast<Orientation>(((unsigned int)(o)+1)%4));

    Block & bLeft(m_blocks(sf::Vector2u(relative(sf::Vector2i(pos), oSide, -1))));
    Block & bRight(m_blocks(relative(pos, oSide, 1u)));

    if(bLeft.wallID == wallLineID || bLeft.wallID == wallLineNoflipID)
    {
        bLeft.wallID = wallCornerID;
        bLeft.wallOrientation = createOrientation(static_cast<Rotation>(((unsigned int)(rot)+3)%4), false, false);
    }
    else if(bLeft.wallID == wallCornerID)
    {
        bLeft.wallID = wallLineID;
        bLeft.wallOrientation = createOrientation(static_cast<Rotation>(oSide), true, false);
    }

    if(bRight.wallID == wallLineID || bRight.wallID == wallLineNoflipID)
    {
        bRight.wallID = wallCornerID;
        bRight.wallOrientation = createOrientation(static_cast<Rotation>(((unsigned int)(rot)+2)%4), false, false);
    }
    else if(bRight.wallID == wallCornerID)
    {
        bRight.wallID = wallLineID;
        bRight.wallOrientation = createOrientation(static_cast<Rotation>(oSide), false, false);
    }
}

void Room::removeDrawedDoor(sf::Vector2u pos)
{
    //todo if needed
}

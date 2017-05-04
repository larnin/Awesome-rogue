#include "room.h"
#include "Generator/patern.h"

Room::Room(const json & j)
    : Serializable(SERIALIZE_ROOM)
    , m_blocks(sf::Vector2u(j["sizex"], j["sizey"]))
    , m_pos(sf::Vector2i(j["posx"], j["posy"]))
    , m_UID(j["id"])
    , m_discovered(j["discovered"])
    , m_type(RoomType(j["type"].get<int>()))
    , m_renderInfosName(j["render"].get<std::string>())
{
    auto blocks(j.find("blocks"));
    if(blocks->is_array() && blocks->size() >= m_blocks.getSize().x*m_blocks.getSize().y)
    {
        for(unsigned int i(0) ; i < m_blocks.getSize().x ; i++)
            for(unsigned int j(0) ; j < m_blocks.getSize().y ; j++)
            {
                const json & block((*blocks)[i+j*m_blocks.getSize().x]);
                if(!block.is_array() || block.size() < 5)
                    m_blocks(sf::Vector2u(i, j)) = Block();
                else if(block.size() < 7)
                    m_blocks(sf::Vector2u(i, j)) = Block(block[0], block[1], block[2], block[3], block[4]);
                else m_blocks(sf::Vector2u(i, j)) = Block(block[0], block[1], block[2], block[3], block[4], block[5], block[6]);
            }
    }

    auto doors(j.find("doors"));
    if(doors->is_array())
    {
        for(const json jDoor : *doors)
        {
            Door d(Location(sf::Vector2f(jDoor["posx"], jDoor["posy"]), m_UID), Location(sf::Vector2f(jDoor["destx"], jDoor["desty"]), jDoor["destr"].get<int>()));
            m_doors.push_back(d);
        }
    }

    auto pop(j.find("pop"));
    if(pop->is_array())
    {
        for(const json jPop : *pop)
            m_population.push_back(EntityType(jPop.get<int>()));
    }
}

Room::Room(const Patern & p, sf::Vector2i pos, unsigned int id)
    : Serializable(SERIALIZE_ROOM)
    , m_blocks(p.getSize())
    , m_pos(pos)
    , m_UID(id)
    , m_discovered(false)
    , m_type(p.type)
    , m_renderInfosName(p.m_renderInfosName)
{
    for(unsigned int i(0) ; i < p.getSize().x ; i++)
        for(unsigned int j(0) ; j < p.getSize().y ; j++)
            m_blocks(sf::Vector2u(i, j)) = p(sf::Vector2u(i, j));
}

Room::Room(sf::Vector2u size, sf::Vector2i pos, unsigned int id, Block def)
    : Serializable(SERIALIZE_ROOM)
    , m_blocks(size, def)
    , m_pos(pos)
    , m_UID(id)
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

/*Block & Room::operator()(const sf::Vector2u & pos)
{
    modified = true;
    return m_blocks(pos);
}

const Block & Room::operator()(const sf::Vector2u & pos) const
{
    return m_blocks(pos);
}*/

const Block & Room::get(const sf::Vector2u & pos) const
{
    return m_blocks(pos);
}

Block & Room::modify(const sf::Vector2u & pos)
{
    modified = true;
    return m_blocks(pos);
}

void Room::set(const sf::Vector2u & pos, const Block & b)
{
    modified = true;
    m_blocks(pos) = b;
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

    modified = true;
    m_doors.push_back(door);
    drawDoor(door.pos.getBlockPos());
}

void Room::delDoor(const sf::Vector2u & pos)
{
    auto it(std::find_if(m_doors.begin(), m_doors.end(), [pos](auto door){return door.pos.getBlockPos() == pos;}));
    if(it == m_doors.end())
        return;

    modified = true;
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
        r->modify(sf::Vector2u(relative(sf::Vector2i(d.dest.getBlockPos()), d.getOrientation(), -1))).groundID = uncoveredDoorID;
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
               : o == Orientation::RIGHT ? Rotation::ROT_270
               : o == Orientation::DOWN ? Rotation::ROT_180
                                        : Rotation::ROT_90);
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
        Rotation r(o == UP ? ROT_90 : o == LEFT ? ROT_180 : o == DOWN ? ROT_270 : ROT_0);
        bLeft.wallOrientation = createOrientation(r, false, false);
    }
    else if(bLeft.wallID == wallCornerID)
    {
        bLeft.wallID = wallLineID;
        bLeft.wallOrientation = createOrientation(static_cast<Rotation>(oSide), true, false);
    }

    if(bRight.wallID == wallLineID || bRight.wallID == wallLineNoflipID)
    {
        bRight.wallID = wallCornerID;
        Rotation r(o == UP ? ROT_180 : o == LEFT ? ROT_270 : o == DOWN ? ROT_0 : ROT_90);
        bRight.wallOrientation = createOrientation(r, false, false);
    }
    else if(bRight.wallID == wallCornerID)
    {
        bRight.wallID = wallLineID;
        bRight.wallOrientation = createOrientation(static_cast<Rotation>(oSide), false, false);
    }

    modified = true;
}

void Room::removeDrawedDoor(sf::Vector2u pos)
{
    //todo if needed
}

void Room::closeDoors()
{
    modified = true;

    const unsigned int openDoorID(4);
    const unsigned int uncoveredOpenDoorID(5);
    const unsigned int closeDoorID(10);
    const unsigned int uncoveredCloseDoorID(11);

    for(const Door & d : m_doors)
    {
        Block & b(m_blocks(d.pos.getBlockPos()));
        if(b.groundID == openDoorID)
            b.groundID = closeDoorID;
        if(b.groundID == uncoveredOpenDoorID)
            b.groundID = uncoveredCloseDoorID;
        setBoxType(b.boxCaracts, BoxType::FULL);
    }
}

void Room::openDoors()
{
    modified = true;

    const unsigned int openDoorID(4);
    const unsigned int uncoveredOpenDoorID(5);
    const unsigned int closeDoorID(10);
    const unsigned int uncoveredCloseDoorID(11);

    for(const Door & d : m_doors)
    {
        Block & b(m_blocks(d.pos.getBlockPos()));
        if(b.groundID == closeDoorID)
            b.groundID = openDoorID;
        if(b.groundID == uncoveredCloseDoorID)
            b.groundID = uncoveredOpenDoorID;
        setBoxType(b.boxCaracts, BoxType::EMPTY);
    }
}

json Room::serialize() const
{
    json j = {
        {"posx", m_pos.x},
        {"posy", m_pos.y},
        {"id", m_UID},
        {"discovered", m_discovered},
        {"type", m_type},
        {"sizex", m_blocks.getSize().x},
        {"sizey", m_blocks.getSize().y},
        {"render", m_renderInfosName}
    };

    json jBlocks;
    for(const Block & b : m_blocks)
    {
        json jb{b.groundID, b.groundOrientation, b.wallID, b.wallOrientation, b.boxCaracts, b.topID, b.topOrientation};
        jBlocks.push_back(jb);
    }
    j["blocks"] = jBlocks;

    json jDoors;
    for(const Door & d : m_doors)
    {
        auto r(d.pos.getRoom().lock());
        auto rD(d.dest.getRoom().lock());
        if(!r || !rD || r->getID() != getID())
            continue;
        json jd = {
            {"posx", d.pos.getBlockPos().x},
            {"posy", d.pos.getBlockPos().y},
            {"destr", rD->getID()},
            {"destx", d.dest.getBlockPos().x},
            {"desty", d.dest.getBlockPos().y}
        };
        jDoors.push_back(jd);
    }
    j["doors"] = jDoors;

    json jPop;
    for(EntityType t : m_population)
    {
        jPop.push_back(t);
    }
    j["pop"] = jPop;

    return j;
}

std::string Room::getRenderInfosName() const
{
    return m_renderInfosName;
}

#include "entity.h"
#include "Collisions/collisions.h"
#include "Utilities/vect2convert.h"
#include "Events/event.h"
#include "Events/Datas/Entity/evententitychangeroom.h"
#include "Events/Datas/eventdropitem.h"
#include "Events/Datas/File/eventloadfinished.h"
#include "Events/eventgetter.h"
#include "Particules/particulefactory.h"
#include "Particules/Types/particulelifebar.h"
#include "Map/door.h"
#include "Map/room.h"

unsigned int Entity::lastID(0);
std::default_random_engine Entity::m_randEngine;

Entity::Entity(const Location & pos, SerializableType type)
    : Serializable(type)
    , m_pos(pos)
    , m_orientation(0.0f)
    , m_damageable(true)
    , m_maxLife(1.0f)
    , m_life(m_maxLife)
    , m_lifeRegeneration(1.0f)
    , m_maxShield(0.0f)
    , m_shield(m_maxShield)
    , m_shieldRegeneration(5.0f)
    , m_shieldDelay(3.0f)
    , m_timeFromLastDamage(0.0f)
    , m_invincibleTime(0.0f)
    , m_showLifeOnDamage(true)
    , m_killed(false)
    , m_team(Team::MOB_TEAM)
    , m_activeDistance(0)
    , m_UID(lastID++)
    , m_canPassDoor(false)
    , m_knockbackMultiplier(1.0f)
{
    connect<EventLoadFinished>(std::bind(&onLoadFinish, this, _1));
}

Entity::Entity(const json & j, SerializableType type)
    : Entity(Location(sf::Vector2f(j["posx"], j["posy"]), j["room"].get<unsigned int>()), type)
{
    m_speed = sf::Vector2f(j["speedx"], j["speedy"]);
    m_orientation = j["orientation"];
    m_damageable = j["damageable"];
    m_maxLife = j["maxlife"];
    m_life = j["life"];
    m_lifeRegeneration = j["liferegen"];
    m_maxShield = j["maxshield"];
    m_shield = j["shield"];
    m_shieldRegeneration = j["shieldregen"];
    m_shieldDelay = j["shielddelay"];
    m_timeFromLastDamage = j["timefromlastdmg"];
    m_invincibleTime = j["invincibletime"];
    m_showLifeOnDamage = j["showlife"];
    m_killed = j["killed"];
    m_originalBox = HitBox(j["originalbox"]);
    m_currentBox = HitBox(j["box"]);
    m_team = (Team)j["team"].get<int>();
    m_activeDistance = j["activedistance"];
    m_UID = j["uid"];
    m_canPassDoor = j["canpassdoor"];
    m_knockbackMultiplier = j["knockback"];

    auto drops = j["drops"];
    for(const auto & jDrop : drops)
        m_drops.push_back((ItemType)jDrop.get<int>());
}

void Entity::update(const sf::Time & elapsedTime)
{
    float time(elapsedTime.asSeconds());
    m_timeFromLastDamage += time;

    float lifeRegen(m_lifeRegeneration*time);
    if(lifeRegen >= m_maxLife-m_life)
        m_life = m_maxLife;
    else m_life += lifeRegen;

    if(m_timeFromLastDamage > m_shieldDelay)
    {
        float shieldRegen(m_shieldRegeneration*time);
        if(shieldRegen >= m_maxShield-m_shield)
            m_shield = m_maxShield;
        m_shield += shieldRegen;
    }

    updateComportement(elapsedTime);
}

bool Entity::damage(float value, std::weak_ptr<Entity>, const sf::Vector2f & dir)
{
    if(!m_damageable)
        return false;

    if(m_invincibleTime > m_timeFromLastDamage)
        return false;

    push(dir);
    if(m_showLifeOnDamage)
        ParticuleFactory::createSend<ParticleLifeBar>(m_pos, EventGetter<std::shared_ptr<Entity>, unsigned int>::get(getID()));

    m_timeFromLastDamage = 0;

    if(m_shield > 0)
    {
        m_shield -= value;
        value = -m_shield;
        if(m_shield > 0)
            return true;
        m_shield = 0;
    }

    m_life -= value;
    if(m_life < 0)
    {
        std::uniform_real_distribution<float> d(-itemDropSpeed, itemDropSpeed);
        for(ItemType i : m_drops)
        {
            sf::Vector2f speed(d(m_randEngine), d(m_randEngine));
            Event<EventDropItem>::send(EventDropItem(i, m_pos, speed));
        }

        m_life = 0;
        m_killed = true;
        onKill();
    }
    return true;
}

bool Entity::damageable()
{
    return m_damageable;
}

void Entity::push(const sf::Vector2f & dir)
{
    m_speed += dir*m_knockbackMultiplier;
}

float Entity::getLife() const
{
    return m_life;
}

float Entity::getMaxLife() const
{
    return m_maxLife;
}
float Entity::getShield() const
{
    return m_shield;
}
float Entity::getMaxShield() const
{
    return m_maxShield;
}

bool Entity::isKilled() const
{
    return m_killed;
}

Team Entity::getTeam() const
{
    return m_team;
}

Location Entity::getPos() const
{
    return m_pos;
}

void Entity::jumpTo(const Location & pos)
{
    m_pos = pos;
    Event<EventEntityChangeRoom>::send(EventEntityChangeRoom(getID()));
}

unsigned int Entity::getActiveDistance() const
{
    return m_activeDistance;
}

bool Entity::canPassDoor() const
{
    return m_canPassDoor;
}

unsigned int Entity::getID() const
{
    return m_UID;
}

float Entity::getOrientation() const
{
    return m_orientation;
}

void Entity::setOrientation(float value)
{
    m_orientation = value;
    m_currentBox = m_originalBox.transform(m_orientation, false, false);
}

void Entity::rotate(float value)
{
    setOrientation(m_orientation+value);
}

HitBox Entity::getBox() const
{
    return m_currentBox;
}

std::vector<ItemType> Entity::getDrops() const
{
    return m_drops;
}

void Entity::execMove(const sf::Vector2f & dir)
{
    sf::Vector2f residualSpeed(dir);

    for(unsigned int i(0) ; i < 2; i++)
    {
        Collisions::InteractResult result(Collisions::interact(m_currentBox, m_pos.getPos(), residualSpeed, m_pos.getRoom().lock()));
        if(!result.collision)
        {
            m_pos.move(residualSpeed);
            break;
        }
        else
        {
            sf::Vector2f localMove(result.endPos-m_pos.getPos());
            m_pos.move(localMove);
            float speedMultiplier(std::cos(result.surfaceAngle - angle(residualSpeed)));
            residualSpeed -= localMove;
            residualSpeed = toVect(norm(residualSpeed)*speedMultiplier, result.surfaceAngle);
            m_speed = toVect(norm(m_speed)*speedMultiplier, result.surfaceAngle);
        }
    }

    if(m_canPassDoor)
    {
        std::shared_ptr<Room> r(m_pos.getRoom().lock());
        if(!r)
            return;
        Door d(r->doorAt(m_pos.getBlockPos()));
        if(!d.isValid())
            return;
        m_pos = d.dest;
        Event<EventEntityChangeRoom>::send(EventEntityChangeRoom(getID()));
    }
}

void Entity::execRotate(float newAngle)
{
    if(Collisions::canRotate(m_currentBox, m_pos.getPos(), newAngle - m_orientation, m_pos.getRoom().lock()))
    {
        m_orientation = newAngle;
        m_currentBox = m_originalBox.transform(m_orientation, false, false);
    }
}

void Entity::onKill()
{

}

json Entity::serialize() const
{
    json j;
    j["posx"] = m_pos.getPos().x;
    j["posy"] = m_pos.getPos().y;
    auto room = m_pos.getRoom().lock();
    if(room)
        j["room"] = room->getID();
    else j["room"] = 0;
    j["speedx"] = m_speed.x;
    j["speedy"] = m_speed.y;
    j["orientation"] = m_orientation;
    j["damageable"] = m_damageable;
    j["maxlife"] = m_maxLife;
    j["life"] = m_life;
    j["liferegen"] = m_lifeRegeneration;
    j["maxshield"] = m_maxShield;
    j["shield"] = m_shield;
    j["shieldregen"] = m_shieldRegeneration;
    j["shielddelay"] = m_shieldDelay;
    j["timefromlastdmg"] = m_timeFromLastDamage;
    j["invincibletime"] = m_invincibleTime;
    j["showlife"] = m_showLifeOnDamage;
    j["killed"] = m_killed;
    j["originalbox"] = m_originalBox.toJson();
    j["box"] = m_currentBox.toJson();
    j["team"] = (int)m_team;
    j["activedistance"] = m_activeDistance;
    j["uid"] = m_UID;
    j["canpassdoor"] = m_canPassDoor;
    j["knockback"] = m_knockbackMultiplier;

    json jDrops;
    for(ItemType i : m_drops)
        jDrops.push_back((int)i);
    j["drops"] = jDrops;

    return j;
}

void Entity::onLoadFinish(EventLoadFinished)
{
    m_pos = Location(m_pos.getPos(), EventGetter<std::shared_ptr<Room>, unsigned int>::get(m_pos.getRoomID()));
}

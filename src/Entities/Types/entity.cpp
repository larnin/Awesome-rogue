#include "entity.h"
#include "Collisions/collisions.h"
#include "Utilities/vect2convert.h"

#include "Events/event.h"
#include "Events/Datas/evententitychangeroom.h"

unsigned int Entity::lastID(0);
std::default_random_engine Entity::m_randEngine;

Entity::Entity(const Location & pos)
    : m_pos(pos)
    , m_orientation(0.0f)
    , m_damageable(true)
    , m_maxLife(1.0f)
    , m_life(m_maxLife)
    , m_maxShield(0.0f)
    , m_shield(m_maxShield)
    , m_killed(false)
    , m_team(Team::MOB_TEAM)
    , m_UID(lastID++)

{

}

void Entity::damage(float value)
{
    if(!m_damageable)
        return;

    if(m_shield > 0)
        m_shield -= value;

    if(m_shield >= 0)
        return;
    value = -m_shield;
    m_shield = 0;

    m_life -= value;
    if(m_life < 0)
    {
        m_life = 0;
        m_killed = true;
    }
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

Location Entity::getPos() const
{
    return m_pos;
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

void Entity::execMove()
{
    sf::Vector2f residualSpeed(m_speed);

    for(unsigned int i(0) ; i < 2; i++)
    {
        Collisions::InteractResult result(Collisions::interact(m_currentBox, m_pos.getPos(), m_speed, m_pos.getRoom().lock()));
        if(!result.collision)
        {
            m_pos.move(m_speed);
            break;
        }
        else
        {
            sf::Vector2f localMove(result.endPos-m_pos.getPos());
            m_pos.move(localMove);
            float a(result.surfaceAngle - angle(m_speed));
            m_speed = toVect(norm(m_speed)*std::cos(a), result.surfaceAngle);
            residualSpeed -= localMove;
            residualSpeed = toVect(norm(residualSpeed)*std::cos(a), result.surfaceAngle);
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
        //m_speed = sf::Vector2f(0, 0);
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

#ifndef COMPORTEMENT_H
#define COMPORTEMENT_H

#include "Systemes/updatable.h"
#include "Utilities/noncopiable.h"
#include "Map/location.h"
#include "Entities/team.h"
#include "Items/itemtype.h"
#include "File/serializable.h"
#include "Events/eventreceiver.h"
#include <SFML/Graphics/Drawable.hpp>
#include <random>

class EventLoadFinished;

class Entity : public Updatable, public sf::Drawable, public Serializable, private NonCopiable, public EventReceiver
{
public:
    Entity(const Location & pos):Entity(pos, SERIALIZE_NONE) {}
    Entity(const Location & pos, SerializableType type);
    Entity(const json & j, SerializableType type);
    Entity(Entity&&) = default;
    Entity & operator= (Entity &&) = default;
    virtual ~Entity() = default;

    bool damage(float value, std::weak_ptr<Entity>, const sf::Vector2f & dir = sf::Vector2f(0, 0));
    bool damageable();
    void push(const sf::Vector2f & dir);
    float getLife() const;
    float getMaxLife() const;
    float getShield() const;
    float getMaxShield() const;
    bool isKilled() const;
    Team getTeam() const;

    unsigned int getActiveDistance() const;
    bool canPassDoor() const;
    unsigned int getID() const;

    Location getPos() const;
    void jumpTo(const Location & pos);
    float getOrientation() const;
    void setOrientation(float value);
    void rotate(float value);

    HitBox getBox() const;

    std::vector<ItemType> getDrops() const;

    void update(const sf::Time & elapsedTime);

protected:
    virtual json serialize() const;

    virtual void updateComportement(const sf::Time & elapsedTime) = 0;
    virtual void onKill();

    void execMove(const sf::Vector2f & dir);
    void execRotate(float newAngle);

    Location m_pos;
    sf::Vector2f m_speed;
    float m_orientation;
    bool m_damageable;
    float m_maxLife;
    float m_life;
    float m_lifeRegeneration;
    float m_maxShield;
    float m_shield;
    float m_shieldRegeneration;
    float m_shieldDelay;
    float m_timeFromLastDamage;
    float m_invincibleTime;
    bool m_showLifeOnDamage;
    bool m_killed;
    HitBox m_originalBox;
    HitBox m_currentBox;
    Team m_team;
    unsigned int m_activeDistance;
    unsigned int m_UID;
    bool m_canPassDoor;
    std::vector<ItemType> m_drops;

    float m_knockbackMultiplier;

    static unsigned int lastID;
    static std::default_random_engine m_randEngine;

private:
    void onLoadFinish(EventLoadFinished);
};

#endif // COMPORTEMENT_H

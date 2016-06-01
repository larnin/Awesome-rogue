#ifndef COMPORTEMENT_H
#define COMPORTEMENT_H

#include <random>
#include <SFML/Graphics/Drawable.hpp>
#include "Systemes/updatable.h"
#include "Utilities/noncopiable.h"
#include "Map/location.h"
#include "Collisions/hitbox.h"

enum Team
{
    PLAYER_TEAM,
    MOB_TEAM
};

class Entity : public Updatable, public sf::Drawable, public NonCopiable
{
public:
    Entity(const Location & pos);
    Entity(Entity&&) = default;
    Entity & operator= (Entity &&) = default;
    virtual ~Entity() = default;

    void damage(float value);
    float getLife() const;
    float getMaxLife() const;
    float getShield() const;
    float getMaxShield() const;
    bool isKilled() const;

    unsigned int getActiveDistance() const;
    bool canPassDoor() const;
    unsigned int getID() const;

    Location getPos() const;
    float getOrientation() const;
    void setOrientation(float value);
    void rotate(float value);

    HitBox getBox() const;

protected:
    void execMove();
    void execRotate(float newAngle);

    Location m_pos;
    sf::Vector2f m_speed;
    float m_orientation;
    bool m_damageable;
    float m_maxLife;
    float m_life;
    float m_maxShield;
    float m_shield;
    bool m_killed;
    HitBox m_originalBox;
    HitBox m_currentBox;
    Team m_team;
    unsigned int m_activeDistance;
    unsigned int m_UID;
    bool m_canPassDoor;

    static unsigned int lastID;
    static std::default_random_engine m_randEngine;
};

#endif // COMPORTEMENT_H

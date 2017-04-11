#ifndef CIRCLEMOB_H
#define CIRCLEMOB_H

#include "entity.h"
#include "Collisions/path.h"
#include "Utilities/ressource.h"
#include <memory>

class Projectile;
class EventPlayerChangeRoom;

class CircleMob : public Entity
{
public:
    CircleMob(const Location & pos, bool small = false);
    CircleMob(const json & j);
    CircleMob(CircleMob&&) = default;
    CircleMob & operator= (CircleMob &&) = default;
    virtual ~CircleMob() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

protected:
    virtual void updateComportement(const sf::Time & elapsedTime);
    virtual void onKill();
    virtual void onAwake();
    virtual void onDisable();

    virtual json serialize() const;

private:
    void onPlayerChangeRoom(EventPlayerChangeRoom e);
    void recreatePath();

    Path m_path;
    Texture m_texture;
    std::weak_ptr<Entity> m_target;

    std::weak_ptr<Projectile> m_projectile;

    float m_lastRandDirTime;
    float m_randAngle;
    bool m_isSmall;
};

#endif // CIRCLEMOB_H

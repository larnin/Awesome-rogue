#ifndef CIRCLEMOB_H
#define CIRCLEMOB_H

#include "entity.h"
#include "Events/eventreceiver.h"
#include "Collisions/path.h"
#include "Utilities/ressource.h"
#include <memory>

class Projectile;
class EventPlayerChangeRoom;

class CircleMob : public Entity, public EventReceiver
{
public:
    CircleMob(const Location & pos, bool small = false);
    CircleMob(CircleMob&&) = default;
    CircleMob & operator= (CircleMob &&) = default;
    virtual ~CircleMob() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

protected:
    virtual void updateComportement(const sf::Time & elapsedTime);
    virtual void onKill();
    virtual void onAwake();
    virtual void onDisable();

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

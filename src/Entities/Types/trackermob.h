#ifndef TRACKER_H
#define TRACKER_H

#include "entity.h"
#include "Utilities/ressource.h"
#include "Events/eventreceiver.h"
#include "Collisions/path.h"

class EventPlayerChangeRoom;

class TrackerMob : public Entity, public EventReceiver
{
public:
    TrackerMob(const Location & pos);
    TrackerMob(TrackerMob&&) = default;
    TrackerMob & operator= (TrackerMob &&) = default;
    virtual ~TrackerMob() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

protected:
    virtual void updateComportement(const sf::Time & elapsedTime);
    virtual void onAwake();
    virtual void onDisable();
    virtual void onKill();

private:
    void onPlayerChangeRoom(EventPlayerChangeRoom e);
    void recreatePath();

    Texture m_texture;
    std::weak_ptr<Entity> m_target;
    Location m_targetPos;
    Path m_path;
    bool m_canfire;
    unsigned int m_projectilesToFire;
    float m_timeToFire;
};

#endif // TRACKER_H

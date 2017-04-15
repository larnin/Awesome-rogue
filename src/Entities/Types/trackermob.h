#ifndef TRACKER_H
#define TRACKER_H

#include "entity.h"
#include "Utilities/ressource.h"
#include "Collisions/path.h"

class EventPlayerChangeRoom;

class TrackerMob : public Entity
{
public:
    TrackerMob(const Location & pos);
    TrackerMob(const json & j);
    TrackerMob(TrackerMob&&) = default;
    TrackerMob & operator= (TrackerMob &&) = default;
    virtual ~TrackerMob() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;

protected:
    virtual void updateComportement(const sf::Time & elapsedTime) override;
    virtual void onAwake() override;
    virtual void onDisable() override;
    virtual void onKill() override;

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

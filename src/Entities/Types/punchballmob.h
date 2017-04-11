#ifndef PUNCHBALLMOB_H
#define PUNCHBALLMOB_H

#include "entity.h"
#include "Utilities/ressource.h"
#include "Collisions/path.h"

class EventPlayerChangeRoom;

class PunchBallMob : public Entity
{
public:
    PunchBallMob(const Location & pos);
    PunchBallMob(const json & j);
    PunchBallMob(PunchBallMob&&) = default;
    PunchBallMob & operator= (PunchBallMob &&) = default;
    virtual ~PunchBallMob() = default;

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
    Path m_path;
    float m_fireTime;
    bool m_onFire;
};

#endif // PUNCHBALLMOB_H

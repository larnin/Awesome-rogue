#ifndef PUNCHBALLMOB_H
#define PUNCHBALLMOB_H

#include "entity.h"
#include "Events/eventreceiver.h"
#include "Utilities/ressource.h"
#include "Collisions/path.h"
#include "Events/Datas/eventplayerchangeroom.h"

class PunchBallMob : public Entity, public EventReceiver
{
public:
    PunchBallMob(const Location & pos);
    PunchBallMob(PunchBallMob&&) = default;
    PunchBallMob & operator= (PunchBallMob &&) = default;
    virtual ~PunchBallMob() = default;

    virtual void update(const sf::Time & elapsedTime);
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

protected:
    virtual void onAwake();
    virtual void onDisable();

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

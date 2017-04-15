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
    Path m_path;
    float m_fireTime;
    bool m_onFire;
};

#endif // PUNCHBALLMOB_H

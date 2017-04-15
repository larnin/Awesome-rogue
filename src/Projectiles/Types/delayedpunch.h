#ifndef DELAYEDPUNCH_H
#define DELAYEDPUNCH_H

#include "projectile.h"
#include "Utilities/ressource.h"

class DelayedPunch : public Projectile
{
public:
    DelayedPunch(const Location & pos, Team team, std::weak_ptr<Entity> sender);
    DelayedPunch(DelayedPunch &&) = default;
    DelayedPunch & operator=(DelayedPunch &&) = default;
    virtual ~DelayedPunch() = default;

    virtual void update(const sf::Time & elapsedTime) override;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;

private:
    void fire();
    Texture m_texture;
    sf::Time m_lifeTime;
    bool m_fired;
};

#endif // DELAYEDPUNCH_H

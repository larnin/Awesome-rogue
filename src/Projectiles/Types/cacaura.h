#ifndef CACAURA_H
#define CACAURA_H

#include "projectile.h"

class CacAura : public Projectile
{
public:
    CacAura(const Location & pos, Team team, std::weak_ptr<Entity> sender, float radius);
    CacAura(CacAura &&) = default;
    CacAura & operator =(CacAura &&) = default;
    virtual ~CacAura() = default;

    virtual void update(const sf::Time &) override;
    virtual void draw(sf::RenderTarget &, sf::RenderStates) const override;

private:
    float m_radius;
};

#endif // CACAURA_H

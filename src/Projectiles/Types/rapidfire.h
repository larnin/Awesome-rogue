#ifndef RAPIDFIRE_H
#define RAPIDFIRE_H

#include "projectile.h"
#include "Utilities/ressource.h"

class RapidFire : public Projectile
{
public:
    RapidFire(const Location & pos, Team team, const sf::Vector2f & speed, std::weak_ptr<Entity> sender);
    RapidFire(RapidFire &&) = default;
    RapidFire & operator =(RapidFire &&) = default;
    virtual ~RapidFire() = default;

    virtual void update(const sf::Time & elapsedTime) override;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;

private:
    virtual HitBox getBox() const override;
    Texture m_texture;
    sf::Vector2f m_speed;
};

#endif // RAPIDFIRE_H

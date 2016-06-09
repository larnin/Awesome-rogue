#ifndef RAPIDFIRE_H
#define RAPIDFIRE_H

#include "projectile.h"
#include "Utilities/ressource.h"

class RapidFire : public Projectile
{
public:
    RapidFire(const Location & pos, Team team, const sf::Vector2f & speed);
    RapidFire(RapidFire &&) = default;
    RapidFire & operator =(RapidFire &&) = default;
    virtual ~RapidFire() = default;

    virtual void update(const sf::Time & elapsedTime);
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    virtual HitBox getBox() const;
    Texture m_texture;
    sf::Vector2f m_speed;
};

#endif // RAPIDFIRE_H

#ifndef LASERMOB_H
#define LASERMOB_H

#include "projectile.h"
#include "Utilities/ressource.h"

class LaserMob : public Projectile
{
public:
    LaserMob(const Location & pos, Team team, std::weak_ptr<Entity> sender, float orientationOffset);
    LaserMob(LaserMob &&) = default;
    LaserMob & operator=(LaserMob &&) = default;
    virtual ~LaserMob() = default;

    virtual void update(const sf::Time & elapsedTime) override;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;

private:
    float maxDistInRect(const sf::Vector2u & size) const;
    Texture m_texture;
    float m_orientationOffset;
    float m_orientation;
    float m_totalTime;
    float m_size;
};

#endif // LASERMOB_H

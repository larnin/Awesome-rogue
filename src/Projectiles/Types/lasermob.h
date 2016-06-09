#ifndef LASERMOB_H
#define LASERMOB_H

#include "projectile.h"
#include "Utilities/ressource.h"
#include <memory>
#include "Entities/Types/entity.h"

class LaserMob : public Projectile
{
public:
    LaserMob(const Location & pos, Team team, std::weak_ptr<Entity> sender, float orientationOffset);
    LaserMob(LaserMob &&) = default;
    LaserMob & operator=(LaserMob &&) = default;
    virtual ~LaserMob() = default;

    virtual void update(const sf::Time & elapsedTime);
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    float maxDistInRect(const sf::Vector2u & size) const;
    Texture m_texture;
    float m_orientationOffset;
    float m_orientation;
    std::weak_ptr<Entity> m_sender;
    float m_totalTime;
};

#endif // LASERMOB_H

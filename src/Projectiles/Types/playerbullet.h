#ifndef PLAYERBULLET_H
#define PLAYERBULLET_H

#include "projectile.h"
#include "Utilities/ressource.h"

class PlayerBullet : public Projectile
{
public:
    PlayerBullet(const Location & pos, Team team, const sf::Vector2f & speed, std::weak_ptr<Entity> sender);
    PlayerBullet(PlayerBullet &&) = default;
    PlayerBullet & operator =(PlayerBullet &&) = default;
    virtual ~PlayerBullet() = default;

    virtual void update(const sf::Time & elapsedTime);
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    virtual HitBox getBox() const;
    Texture m_texture;
    sf::Vector2f m_speed;
};

#endif // PLAYERBULLET_H

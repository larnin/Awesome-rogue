#ifndef BIGCIRCLE_H
#define BIGCIRCLE_H

#include "projectile.h"
#include "Utilities/ressource.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>


class BigCircle : public Projectile
{
public:
    BigCircle(const Location & pos, Team team, const sf::Vector2f & speed, float maxTime, std::weak_ptr<Entity> sender);
    BigCircle(BigCircle &&) = default;
    BigCircle & operator=(BigCircle &&) = default;
    virtual ~BigCircle() = default;

    virtual void update(const sf::Time & elapsedTime) override;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;

private:
    virtual HitBox getBox() const override;
    Texture m_texture;
    sf::Vector2f m_speed;
    float m_lastTime;
};

#endif // BIGCIRCLE_H

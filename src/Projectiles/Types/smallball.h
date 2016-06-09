#ifndef SMALLBALL_H
#define SMALLBALL_H

#include "projectile.h"
#include "Utilities/ressource.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

class SmallBall : public Projectile
{
public:
    SmallBall(const Location & pos, Team team, const sf::Vector2f & speed, unsigned int bounces, float maxTime);
    SmallBall(SmallBall &&) = default;
    SmallBall & operator=(SmallBall &&) = default;
    virtual ~SmallBall() = default;

    virtual void update(const sf::Time & elapsedTime);
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    virtual HitBox getBox() const;
    Texture m_texture;
    sf::Vector2f m_speed;
    unsigned int m_bounces;
    float m_lastTime;
};

#endif // SMALLBALL_H

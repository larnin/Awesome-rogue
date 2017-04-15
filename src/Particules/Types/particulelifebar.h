#ifndef PARTICULELIFEBAR_H
#define PARTICULELIFEBAR_H

#include "particule.h"
#include "Utilities/ressource.h"

class Entity;

class ParticleLifeBar : public Particule
{
public:
    ParticleLifeBar(const Location & pos, std::weak_ptr<Entity> entity);
    ParticleLifeBar(ParticleLifeBar &&) = default;
    ParticleLifeBar & operator =(ParticleLifeBar &&) = default;
    virtual ~ParticleLifeBar() = default;

    virtual void update(const sf::Time & elapsedTime) override;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;

private:
    Texture m_texture;
    std::weak_ptr<Entity> m_entity;
    float m_lifeTime;
};

#endif // PARTICULELIFEBAR_H

#include "particulelifebar.h"
#include "Entities/Types/entity.h"
#include "Utilities/quadrender.h"
#include "Map/blocktype.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

ParticleLifeBar::ParticleLifeBar(const Location & pos, std::weak_ptr<Entity> entity)
    : Particule(pos)
    , m_texture("res/img/lifeParticle.png")
    , m_entity(entity)
    , m_lifeTime(0)
{

}

void ParticleLifeBar::update(const sf::Time & elapsedTime)
{
    if(m_killed)
        return;

    const float maxLife(3);
    m_lifeTime += elapsedTime.asSeconds();
    if(m_lifeTime >= maxLife)
        m_killed = true;

    std::shared_ptr<Entity> e(m_entity.lock());
    if(!e)
    {
        m_killed = true;
        return;
    }
    m_pos = e->getPos();
}

void ParticleLifeBar::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    if(m_killed)
        return;

    std::shared_ptr<Entity> e(m_entity.lock());
    if(!e)
        return;

    float shieldSize(0);
    if(e->getMaxShield() > 0)
        shieldSize = e->getShield()/e->getMaxShield();
    float lifeSize(0);
    if(e->getMaxLife() > 0)
        lifeSize = e->getLife()/e->getMaxLife();
    const float verticalOffset(-10.0f);
    sf::Vector2f centerPos(m_pos.toGlobalPos()*float(BlockType::tileSize)+sf::Vector2f(0, verticalOffset));
    sf::Vector2f size(16, 2);

    sf::VertexArray array(sf::Quads, 12);
    drawQuad(&array[0], sf::FloatRect(centerPos-size/2.0f, size), sf::FloatRect(sf::Vector2f(0, 0), size));
    drawQuad(&array[4], sf::FloatRect(centerPos-size/2.0f, sf::Vector2f(size.x*lifeSize, size.y)), sf::FloatRect(0, 2, size.x*lifeSize, size.y));
    drawQuad(&array[8], sf::FloatRect(centerPos-size/2.0f, sf::Vector2f(size.x*shieldSize, size.y)), sf::FloatRect(0, 4, size.x*lifeSize, size.y));

    target.draw(array, sf::RenderStates(m_texture()));
}

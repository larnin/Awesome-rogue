#ifndef SPAWNBOSS1_H
#define SPAWNBOSS1_H

#include "particule.h"
#include "Utilities/ressource.h"
#include <SFML/Graphics/VertexArray.hpp>

class SpawnBoss1 : public Particule
{
public:
    SpawnBoss1(const Location & pos, float timeToSpawn, float radius);
    SpawnBoss1(SpawnBoss1 &&) = default;
    SpawnBoss1 & operator =(SpawnBoss1 &&) = default;
    virtual ~SpawnBoss1() = default;

    virtual void update(const sf::Time & elapsedTime);
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    void drawBossPart(const sf::Vector2f & pos, sf::VertexArray & array) const;

    float m_time;
    float m_maxTime;
    float m_radius;

    Texture m_bossTexture;
    Texture m_laserTexture;

    std::vector<unsigned int> m_offsetBeam;
};

#endif // SPAWNBOSS1_H

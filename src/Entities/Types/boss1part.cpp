#include "boss1part.h"
#include "Utilities/vect2convert.h"
#include "Particules/particulefactory.h"
#include "Particules/Types/mobdeath.h"
#include "Map/blocktype.h"
#include "Utilities/quadrender.h"
#include "Projectiles/Types/smallball.h"
#include "Projectiles/Types/rapidfire.h"
#include "Projectiles/projectilefactory.h"
#include "Utilities/vect2convert.h"
#include "Events/eventgetter.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/VertexArray.hpp>

const float minWaveTime(10.0f);
const float maxWaveTime(50.0f);
const float minRandProjectileTime(1.0f);
const float maxRandProjectileTime(2.5f);
const float delayBullets(0.1f);
const float nbBullets(30.0f);
const float bulletOnCircle(15.0f);
const float projectileSpeed(1.5f);
const float PI(3.14159f);
const float timeDifficultyMultiplier(0.1f);

Boss1Part::Boss1Part(const Location & pos, float startOrientation, float distanceFromCenter)
    : Entity(pos)
    , m_distance(distanceFromCenter)
    , m_texture("res/img/boss1.png")
    , m_lifeTime(0)
{
    m_orientation = startOrientation;
    m_maxLife = 50.0f;
    m_life = m_maxLife;
    m_team = Team::MOB_TEAM;
    m_activeDistance = 0;
    m_canPassDoor = false;
    m_showLifeOnDamage = false;

    m_knockbackMultiplier = 0.0f;

    const float radius(1.0f);
    for(int i(0) ; i < 12 ; i++)
    {
        float a(3.14159/8*(i+0.5f));
        float nA(3.14159/8*(i+1.5f));
        m_originalBox.addLine(Line(sf::Vector2f(radius*std::cos(a), radius*std::sin(a)), sf::Vector2f(radius*std::cos(nA), radius*std::sin(nA))));
    }
    m_currentBox = m_originalBox.transform(toVect(distanceFromCenter, startOrientation));

    m_timeToNextWave = std::uniform_real_distribution<float>(minWaveTime, maxWaveTime)(m_randEngine);
    m_timeToRandomProjectile = std::uniform_real_distribution<float>(minRandProjectileTime, maxRandProjectileTime)(m_randEngine);

    m_drops = experienceToItems(20);
}

void Boss1Part::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    const float radius(m_damageable ? 17.0f : 7.0f);
    const float texOffset(m_damageable ? 70.0f : 104.0f);

    sf::Vector2f pos((m_pos.toGlobalPos()+toVect(m_distance, m_orientation))*float(BlockType::tileSize));

    sf::VertexArray array(sf::Quads, 4);
    drawQuad(&array[0], sf::FloatRect(pos.x-radius, pos.y-radius, 2*radius, 2*radius), sf::FloatRect(texOffset, 0, 2*radius, 2*radius));

    target.draw(array, sf::RenderStates(m_texture()));
}

void Boss1Part::updateComportement(const sf::Time & elapsedTime)
{
    m_lifeTime += elapsedTime.asSeconds();

    const float rotationSpeed(0.2f);

    m_orientation += rotationSpeed*elapsedTime.asSeconds();
    m_currentBox = m_originalBox.transform(toVect(m_distance, m_orientation));

    float time(elapsedTime.asSeconds());

    if(!m_damageable)
        return;

    m_timeToNextWave -= time;
    if(m_timeToNextWave > 0)
        m_timeToRandomProjectile -= time;

    const float timeMultiplier(1+(m_lifeTime*timeDifficultyMultiplier));

    if(m_timeToRandomProjectile <= 0)
    {
        throwRandomProjectile();
        m_timeToRandomProjectile = std::uniform_real_distribution<float>(minRandProjectileTime/timeMultiplier, maxRandProjectileTime/timeMultiplier)(m_randEngine);
    }

    if(m_timeToNextWave <= 0)
        if(waveUpdate(elapsedTime))
            m_timeToNextWave = std::uniform_real_distribution<float>(minWaveTime/timeMultiplier, maxWaveTime/timeMultiplier)(m_randEngine);
}

void Boss1Part::onKill()
{
    sf::Vector2f pos(m_pos.getPos() + toVect(m_distance, m_orientation));
    ParticuleFactory::createSend<MobDeath>(Location(pos, m_pos.getRoom()), m_texture, sf::FloatRect(70, 0, 34, 34), 0, sf::Vector2f(0, 0));

    m_killed = false;
    m_damageable = false;
    m_lifeRegeneration = 0;
}

void Boss1Part::throwRandomProjectile() const
{
    float orientation(std::uniform_real_distribution<float>(-PI,PI)(m_randEngine));

    Location pos(getPos());
    pos.move(toVect(m_distance, m_orientation));
    pos.move(toVect(1, orientation));

    ProjectileFactory::createSend<RapidFire>(pos, m_team, toVect(projectileSpeed*5, orientation)
                                             , EventGetter<std::shared_ptr<Entity>, unsigned int>::get(getID()));
}

bool Boss1Part::waveUpdate(const sf::Time & elapsedTime) const
{
    float nbBulletsOnTime(nbBullets+m_lifeTime*timeDifficultyMultiplier);
    float time(elapsedTime.asSeconds());
    if(m_timeToNextWave+time > 0)
        return false;

    if(-m_timeToNextWave > delayBullets*nbBulletsOnTime)
        return true;

    int id(-m_timeToNextWave/delayBullets);
    int lastID((-m_timeToNextWave-time)/delayBullets);

    if(id == lastID)
        return false;

    float angle(2*PI*id/bulletOnCircle);
    Location pos(getPos());

    pos.move(toVect(m_distance, m_orientation));
    pos.move(toVect(1, angle));
    ProjectileFactory::createSend<SmallBall>(pos, m_team, toVect(projectileSpeed, angle), m_lifeTime*timeDifficultyMultiplier, 20*(1+m_lifeTime*timeDifficultyMultiplier)
                                             , EventGetter<std::shared_ptr<Entity>, unsigned int>::get(getID()));
    return false;
}

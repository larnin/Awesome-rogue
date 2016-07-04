#include "parallelebulletlauncher.h"
#include "Utilities/vect2convert.h"
#include "Projectiles/projectilefactory.h"
#include "Projectiles/Types/playerbullet.h"

ParalleleBulletLauncher::ParalleleBulletLauncher(std::weak_ptr<Entity> holder, unsigned int bulletCount, float time)
    : ProjectileLauncher(holder)
    , m_deltaBulletTime(time)
    , m_timeFromLastFire(0)
    , m_bulletCount(bulletCount)
    , m_fire(false)
{

}

void ParalleleBulletLauncher::control(CommandsValue & v)
{
    m_fireDirection = sf::Vector2f(v.getValue(CommandType::SHOOT_RIGHT) - v.getValue(CommandType::SHOOT_LEFT)
                                   , v.getValue(CommandType::SHOOT_DOWN) - v.getValue(CommandType::SHOOT_UP));
    if(norm(m_fireDirection) < 0.2f)
    {
        m_fire = false;
        return;
    }

    m_fire = true;
    m_fireDirection  = normalise(m_fireDirection)*15.0f;
}

void ParalleleBulletLauncher::update(const sf::Time & elapsedTime)
{
    if(!m_fire)
    {
        m_timeFromLastFire = 0;
        return;
    }
    m_timeFromLastFire += elapsedTime.asSeconds();
    if(m_timeFromLastFire > m_deltaBulletTime)
    {
        m_timeFromLastFire = 0;
        std::shared_ptr<Entity> e(m_holder.lock());
        if(!e)
            return;
        float bulletOffset(0.25f);
        float offsetOrientation(angle(m_fireDirection)+3.14159f/2.0f);

        for(unsigned int i(0) ; i < m_bulletCount ; i++)
        {
            Location pos(e->getPos());
            pos.move(toVect((int(m_bulletCount)-2*int(i)-1)*bulletOffset, offsetOrientation));
            ProjectileFactory::createSend<PlayerBullet>(pos, e->getTeam(), m_fireDirection, e);
        }

    }
}

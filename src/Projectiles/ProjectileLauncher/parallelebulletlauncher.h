#ifndef PARALLELEBULLETLAUNCHER_H
#define PARALLELEBULLETLAUNCHER_H

#include "projectilelauncher.h"
#include <SFML/System/Vector2.hpp>

class ParalleleBulletLauncher : public ProjectileLauncher
{
public:
    ParalleleBulletLauncher(std::weak_ptr<Entity> holder, unsigned int bulletCount, float time);
    ParalleleBulletLauncher(ParalleleBulletLauncher &&) = default;
    ParalleleBulletLauncher & operator =(ParalleleBulletLauncher &&) = default;
    virtual ~ParalleleBulletLauncher() = default;

    virtual void control(CommandsValue & v);
    virtual void update(const sf::Time & elapsedTime);

private:
    float m_deltaBulletTime;
    float m_timeFromLastFire;
    unsigned int m_bulletCount;
    bool m_fire;
    sf::Vector2f m_fireDirection;
};

#endif // PARALLELEBULLETLAUNCHER_H

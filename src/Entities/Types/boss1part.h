#ifndef BOSS1PART_H
#define BOSS1PART_H

#include "entity.h"
#include "Utilities/ressource.h"

class Boss1Part : public Entity
{
public:
    Boss1Part(const Location & pos, float startOrientation, float distanceFromCenter);
    Boss1Part(Boss1Part &&) = default;
    Boss1Part & operator =(Boss1Part &&) = default;
    virtual ~Boss1Part() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

protected:
    virtual void updateComportement(const sf::Time & elapsedTime);
    virtual void onKill();

private:
    void throwRandomProjectile() const;
    bool waveUpdate(const sf::Time & elapsedTime) const;

    float m_distance;
    Texture m_texture;

    float m_timeToNextWave;
    float m_timeToRandomProjectile;
};

#endif // BOSS1PART_H

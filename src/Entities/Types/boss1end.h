#ifndef BOSS1END_H
#define BOSS1END_H

#include "entity.h"
#include "Utilities/ressource.h"

class Particule;

class Boss1End : public Entity
{
public:
    Boss1End(const Location & pos);
    Boss1End(Boss1End &&) = default;
    Boss1End & operator =(Boss1End &&) = default;
    virtual ~Boss1End() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

protected:
    virtual void updateComportement(const sf::Time & elapsedTime);
    virtual void onKill();

private:
    void fire();
    Texture m_texture;
    std::weak_ptr<Entity> m_target;
    float m_time;
};

#endif // BOSS1END_H

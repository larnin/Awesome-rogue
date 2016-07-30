#ifndef BOSS1FINALLIFEBAR_H
#define BOSS1FINALLIFEBAR_H

#include "bosslifebar.h"
#include "Utilities/ressource.h"
#include "Utilities/animation.h"

class Boss1FinalLifeBar : public BossLifeBar
{
public:
    Boss1FinalLifeBar(const std::vector<std::weak_ptr<Entity>> & entities);
    Boss1FinalLifeBar(Boss1FinalLifeBar &&) = default;
    Boss1FinalLifeBar & operator =(Boss1FinalLifeBar &&) = default;
    virtual ~Boss1FinalLifeBar() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;
    virtual void update(const sf::Time & elapsedTime);

private:
    Texture m_texture;
    float m_time;
    Animation m_animation;
};

#endif // BOSS1FINALLIFEBAR_H

#ifndef BOSS1PARTSLIFEBAR_H
#define BOSS1PARTSLIFEBAR_H

#include "bosslifebar.h"
#include "Utilities/ressource.h"
#include "Map/location.h"

class Particule;

class Boss1PartsLifeBar : public BossLifeBar
{
public:
    Boss1PartsLifeBar(const std::vector<std::weak_ptr<Entity>> & entities);
    Boss1PartsLifeBar(Boss1PartsLifeBar &&) = default;
    Boss1PartsLifeBar & operator =(Boss1PartsLifeBar &&) = default;
    virtual ~Boss1PartsLifeBar() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;
    virtual void update(const sf::Time &) override;

private:
    void onEndPart();
    Texture m_texture;
    std::weak_ptr<Particule> m_transition;
    Location m_pos;
    bool m_onTransition;
};

#endif // BOSS1PARTSLIFEBAR_H

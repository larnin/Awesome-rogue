#ifndef BOSSLIFEBAR_H
#define BOSSLIFEBAR_H

#include "Utilities/noncopiable.h"
#include "Systemes/updatable.h"
#include <SFML/Graphics/Drawable.hpp>
#include <vector>
#include <memory>

class Entity;

class BossLifeBar : public sf::Drawable, public Updatable, private NonCopiable
{
public:
    BossLifeBar(const std::vector<std::weak_ptr<Entity>> & entities);
    BossLifeBar(BossLifeBar &&) = default;
    BossLifeBar & operator =(BossLifeBar &&) = default;
    virtual ~BossLifeBar() = default;

    virtual void update(const sf::Time &) override {}

protected:
    std::vector<std::weak_ptr<Entity>> m_entities;
};

#endif // BOSSLIFEBAR_H

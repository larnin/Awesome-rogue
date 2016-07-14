#ifndef BOSSLIFEBAR_H
#define BOSSLIFEBAR_H

#include <SFML/Graphics/Drawable.hpp>
#include "Utilities/noncopiable.h"
#include "Entities/Types/entity.h"
#include <vector>
#include <memory>

class BossLifeBar : public sf::Drawable, private NonCopiable
{
public:
    BossLifeBar(const std::vector<std::weak_ptr<Entity>> & entities);
    BossLifeBar(BossLifeBar &&) = default;
    BossLifeBar & operator =(BossLifeBar &&) = default;
    virtual ~BossLifeBar() = default;

protected:
    std::vector<std::weak_ptr<Entity>> m_entities;
};

#endif // BOSSLIFEBAR_H

#include "bosslifebar.h"
#include "Entities/Types/entity.h"

BossLifeBar::BossLifeBar(const std::vector<std::weak_ptr<Entity>> & entities)
    : m_entities(entities)
{

}

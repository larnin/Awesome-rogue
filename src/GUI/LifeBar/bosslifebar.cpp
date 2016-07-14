#include "bosslifebar.h"

BossLifeBar::BossLifeBar(const std::vector<std::weak_ptr<Entity>> & entities)
    : m_entities(entities)
{

}

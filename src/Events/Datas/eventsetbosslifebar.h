#ifndef EVENTSETBOSSLIFEBAR_H
#define EVENTSETBOSSLIFEBAR_H

#include <memory>
#include "GUI/LifeBar/bosslifebar.h"

struct EventSetBossLifeBar
{
    EventSetBossLifeBar(std::shared_ptr<BossLifeBar> _lifeBar)
        : lifeBar(_lifeBar)
    {

    }

    std::shared_ptr<BossLifeBar> lifeBar;
};

#endif // EVENTSETBOSSLIFEBAR_H

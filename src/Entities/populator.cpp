#include "populator.h"
#include "entityfactory.h"
#include "Events/eventgetter.h"
#include "Entities/Types/entity.h"
#include "Map/room.h"
#include "Particules/particulefactory.h"
#include "Particules/Types/mobspawn.h"
#include "Utilities/vect2convert.h"
#include "Events/Datas/Entity/eventpreplayerchangeroom.h"
#include "Events/Datas/eventinteraction.h"
#include "Events/Datas/Camera/eventplaycameraeffect.h"
#include "Events/Datas/Entity/eventsetbosslifebar.h"
#include "Events/event.h"
#include "Particules/Types/spawnboss1.h"
#include "Utilities/delayedtask.h"
#include "entityfactory.h"
#include "GUI/LifeBar/boss1partslifebar.h"
#include "GUI/LifeBar/boss1finallifebar.h"
#include <algorithm>

Populator::Populator()
    : m_enabled(false)
{
    connect<EventPrePlayerChangeRoom>(std::bind(&Populator::onPlayerChangeRoom, this, _1));
    connect<EventInteraction>(std::bind(&Populator::onSpawnBossInteraction, this, _1));
}

void Populator::enable()
{
    m_enabled = true;

    for(auto & t : m_tasks)
    {
        t->unPause();
        Updatable::add(t);
    }

    cleanTasks();
}

void Populator::disable()
{
    m_enabled = false;

    for(auto & t : m_tasks)
        Updatable::del(t);

    cleanTasks();
}

void Populator::cleanTasks()
{
    auto it(std::remove_if(m_tasks.begin(), m_tasks.end(), [](const auto & t)
    {
        return t->finished();
    }));
    m_tasks.erase(it, m_tasks.end());
}

void Populator::onPlayerChangeRoom(EventPrePlayerChangeRoom e)
{
    std::shared_ptr<Entity> player(EventGetter<std::shared_ptr<Entity>, unsigned int>::get(e.entityID));
    if(!player)
        return;

    std::shared_ptr<Room> r(player->getPos().getRoom().lock());
    if(!r)
        return;
    std::uniform_int_distribution<unsigned int> dX(1, r->getSize().x-2);
    std::uniform_int_distribution<unsigned int> dY(1, r->getSize().y-2);
    std::uniform_real_distribution<float> dTime(1.5f, 2.5f);

    auto population(r->getAndResetPopulation());

    for(EntityType type : population)
    {
        for(unsigned int i(0) ; i < 10 ; i++)
        {
            bool posOk(true);
            sf::Vector2u pos(dX(m_rand), dY(m_rand));
            for(int u(-1) ; u <= 1 ; u++)
                for(int v(-1) ; v <= 1 ; v++)
                {
                    sf::Vector2u dPos(pos.x+u, pos.y+v);
                    if(getBoxType(r->get(dPos).boxCaracts) != EMPTY)
                    {
                        posOk = false;
                        break;
                    }
                }

            if(!posOk)
                continue;

            Location l(pos, r);

            float time(dTime(m_rand));

            m_tasks.push_back(std::make_shared<DelayedTask>([type, l](){EntityFactory::create(type, l);}, time, m_enabled));
            Updatable::add(m_tasks.back());
            m_tasks.push_back(std::make_shared<DelayedTask>([time, l](){ParticuleFactory::createSend<MobSpawn>(l, time);}, 0.1f, m_enabled));
            Updatable::add(m_tasks.back());
            break;
        }
    }
    cleanTasks();
}

void Populator::onSpawnBossInteraction(EventInteraction e)
{
    switch(e.type)
    {
    case BlockInteractionType::BI_START_BOSS1:
        startBoss1(e.pos);
    break;
    case BlockInteractionType::BI_START_BOSS1_FINAL:
        startBoss1Final(e.pos);
    break;
    default:
    break;
    }
}

void Populator::startBoss1(const Location & pos)
{
    const unsigned int blockUsedBoss1(514);

    auto r(pos.getRoom().lock());
    if(!r)
        return;
    Event<EventPlayCameraEffect>::send(EventPlayCameraEffect(CameraEffectType::EFFECT_HARD_SHAKE, 0.1f));
    Event<EventPlayCameraEffect>::send(EventPlayCameraEffect(CameraEffectType::EFFECT_VERY_LOW_SHAKE, 6.0f));
    r->closeDoors();
    r->modify(pos.getBlockPos()).groundID = blockUsedBoss1;
    ParticuleFactory::createSend<SpawnBoss1>(pos, 6, 7);

    m_tasks.push_back(std::make_shared<DelayedTask>([pos]()
    {
        auto eList(EntityFactory::create(EntityType::E_BOSS1_PARTS, pos));
        std::vector<std::weak_ptr<Entity>> eWList;
        for(auto & eS : eList)
            eWList.push_back(eS);
        auto lifeBar(std::make_shared<Boss1PartsLifeBar>(eWList));
        Event<EventSetBossLifeBar>::send(EventSetBossLifeBar(lifeBar));
    }, 6, m_enabled));
    Updatable::add(m_tasks.back());
}

void Populator::startBoss1Final(const Location & pos)
{
    auto e(EntityFactory::create(EntityType::E_BOSS1_FINAL, pos));
    std::vector<std::weak_ptr<Entity>> eWList;
    for(auto & eS : e)
        eWList.push_back(eS);
    Event<EventSetBossLifeBar>::send(EventSetBossLifeBar(std::make_shared<Boss1FinalLifeBar>(eWList)));
}

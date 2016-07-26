#include "populator.h"
#include "entityfactory.h"
#include <algorithm>
#include "Events/eventgetter.h"
#include "Entities/Types/entity.h"
#include "Map/room.h"
#include "Particules/particulefactory.h"
#include "Particules/Types/mobspawn.h"
#include "Utilities/vect2convert.h"
#include "Events/Datas/eventpreplayerchangeroom.h"
#include "Events/Datas/eventinteraction.h"
#include "Events/Datas/eventplaycameraeffect.h"
#include "Particules/Types/spawnboss1.h"
#include "Utilities/delayedtask.h"
#include "entityfactory.h"

Populator::Populator()
    : m_enabled(false)
{
    connect<EventPrePlayerChangeRoom>(std::bind(&Populator::onPlayerChangeRoom, this, _1));
    connect<EventInteraction>(std::bind(&Populator::onSpawnBossInteraction, this, _1));
}

Populator::~Populator()
{
    for(auto & t : m_tasks)
    {
        std::shared_ptr<DelayedTask> task(t.lock());
        if(task)
            task->stop();
    }
}

void Populator::enable()
{
    m_enabled = true;

    for(auto & t : m_tasks)
    {
        std::shared_ptr<DelayedTask> task(t.lock());
        if(task)
            task->unpause();
    }

    cleanTasks();
}

void Populator::disable()
{
    for(auto & t : m_tasks)
    {
        std::shared_ptr<DelayedTask> task(t.lock());
        if(task)
            task->pause();
    }

    cleanTasks();
}

void Populator::cleanTasks()
{
    auto it(std::remove_if(m_tasks.begin(), m_tasks.end(), [](const auto & t)
    {
        std::shared_ptr<DelayedTask> task(t.lock());
        return !task;
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
                    if(getBoxType((*r)(dPos).boxCaracts) != EMPTY)
                    {
                        posOk = false;
                        break;
                    }
                }

            if(!posOk)
                continue;

            Location l(pos, r);

            float time(dTime(m_rand));

            m_tasks.push_back(DelayedTask::create([type, l](){EntityFactory::create(type, l);}, time, m_enabled));
            cleanTasks();

            m_tasks.push_back(DelayedTask::create([time, l](){ParticuleFactory::createSend<MobSpawn>(l, time);}, 0.1f, m_enabled));
            break;
        }
    }
}

void Populator::onSpawnBossInteraction(EventInteraction e)
{
    const unsigned int blockUsedBoss1(514);

    if(e.type != BlockInteractionType::BI_START_BOSS1)
        return;

    auto r(e.pos.getRoom().lock());
    if(!r)
        return;
    Event<EventPlayCameraEffect>::send(EventPlayCameraEffect(CameraEffectType::EFFECT_HARD_SHAKE, 0.1f));
    Event<EventPlayCameraEffect>::send(EventPlayCameraEffect(CameraEffectType::EFFECT_VERY_LOW_SHAKE, 6.0f));
    r->closeDoors();
    (*r)(e.pos.getBlockPos()).groundID = blockUsedBoss1;
    ParticuleFactory::createSend<SpawnBoss1>(e.pos, 6, 7);

    m_tasks.push_back(DelayedTask::create([e](){EntityFactory::create(EntityType::E_BOSS1_PARTS, e.pos);}, 6, m_enabled));
}

#include "populator.h"
#include "entityfactory.h"
#include <algorithm>
#include "Events/eventgetter.h"
#include "Entities/Types/entity.h"
#include "Map/room.h"
#include "Particules/particulefactory.h"
#include "Particules/Types/mobspawn.h"
#include "Utilities/vect2convert.h"

Populator::Populator()
{
    connect<EventPrePlayerChangeRoom>(std::bind(&Populator::onPlayerChangeRoom, this, _1));
}

void Populator::update(const sf::Time & elapsedTime)
{
    const float time(elapsedTime.asSeconds());
    for(auto & data : m_toSpawn)
    {
        data.timeToSpawn -= time;
        if(data.timeToSpawn <= 0)
            EntityFactory::create(data.type, data.pos);
    }
    auto it(std::remove_if(m_toSpawn.begin(), m_toSpawn.end(), [](const auto & v){return v.timeToSpawn <= 0;}));
    m_toSpawn.erase(it, m_toSpawn.end());
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
    std::uniform_real_distribution<float> dTime(2.5f, 3.5f);

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

            for(const auto & p : m_toSpawn)
            {
                std::shared_ptr<Room> room(p.pos.getRoom().lock());
                if(!room)
                    continue;
                if(room != r)
                    continue;
                if(norm(p.pos.getPos()-sf::Vector2f(pos)) < 1)
                {
                    posOk = false;
                    break;
                }
            }
            if(!posOk)
                continue;

            float time(dTime(m_rand));

            m_toSpawn.push_back(Populator::PopulatorData(type, l, time));
            ParticuleFactory::createSend<MobSpawn>(l, time);
            break;
        }
    }
}

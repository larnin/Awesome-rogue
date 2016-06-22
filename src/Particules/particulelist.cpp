#include "particulelist.h"
#include <cassert>
#include "Systemes/drawablelist.h"
#include "Entities/entitylist.h"
#include "Map/room.h"
#include <algorithm>
#include "Events/eventgetter.h"

const unsigned int particuleHeight(5);

bool ParticuleList::m_instanced(false);

ParticuleList::ParticuleList()
{
    assert(!m_instanced);
    m_instanced = true;

    connect<EventPrePlayerChangeRoom>(std::bind(&ParticuleList::onPlayerChangeRoom, this, _1));
    connect<EventParticuleCreated>(std::bind(&ParticuleList::onParticuleCreated, this, _1));

    EventSimpleGetter<std::vector<std::shared_ptr<Particule>>>::connect(std::bind(&ParticuleList::particules, this));
}

ParticuleList::~ParticuleList()
{
    EventSimpleGetter<std::vector<std::shared_ptr<Particule>>>::disconnect();
    m_instanced = false;
}

void ParticuleList::addParticule(std::shared_ptr<Particule> p)
{
    if(!p)
        return;
    std::shared_ptr<Room> r(p->getPos().getRoom());
    if(!r)
        return;
    if(r->getID() != m_playerRoom)
        return;

    if(std::find(m_particules.begin(), m_particules.end(), p) != m_particules.end())
        return;
    m_particules.push_back(p);

    DrawableList::add(p, particuleHeight);
    Updatable::add(p);
}

void ParticuleList::removeParticule(std::shared_ptr<Particule> p)
{
    if(!p)
        return;

    auto it = std::find(m_particules.begin(), m_particules.end(), p);
    if(it == m_particules.end())
        return;

    std::swap(*it, m_particules.back());
    m_particules.pop_back();
}

const std::vector<std::shared_ptr<Particule>> ParticuleList::particules() const
{
    return m_particules;
}

void ParticuleList::clean()
{
    auto it(std::remove_if(m_particules.begin(), m_particules.end(), [](const auto & p){if(!p) return true; return p->isKilled();}));
    m_particules.erase(it, m_particules.end());
}

void ParticuleList::clear()
{
    m_particules.clear();
}

void ParticuleList::onPlayerChangeRoom(EventPrePlayerChangeRoom e)
{
    clear();
    std::shared_ptr<Entity> entity(EventGetter<std::shared_ptr<Entity>, unsigned int>::get(e.entityID));
    if(entity)
    {
        std::shared_ptr<Room> room(entity->getPos().getRoom().lock());
        if(room)
            m_playerRoom = room->getID();
    }
}

void ParticuleList::onParticuleCreated(EventParticuleCreated e)
{
    addParticule(e.particule);
}

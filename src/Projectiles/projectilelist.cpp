#include "projectilelist.h"
#include "Systemes/drawablelist.h"
#include "Entities/entitylist.h"
#include "Map/room.h"
#include "Events/eventgetter.h"
#include "Entities/Types/entity.h"
#include "Events/Datas/eventprojectilecreated.h"
#include "Events/Datas/Entity/eventpreplayerchangeroom.h"
#include <algorithm>

bool ProjectileList::m_instanced(false);

ProjectileList::ProjectileList()
    : m_playerRoom(0)
    , m_enabled(false)
{
    assert(!m_instanced);
    m_instanced = true;

    connect<EventPrePlayerChangeRoom>(std::bind(&ProjectileList::onPlayerChangeRoom, this, _1));
    connect<EventProjectileCreated>(std::bind(&ProjectileList::onProjectileCreated, this, _1));

    EventSimpleGetter<std::vector<std::shared_ptr<Projectile>>>::connect(std::bind(&ProjectileList::projectiles, this));
}

ProjectileList::~ProjectileList()
{
    EventSimpleGetter<std::vector<std::shared_ptr<Projectile>>>::disconnect();
    m_instanced = false;
}


void ProjectileList::enable()
{
    m_enabled = true;

    for(const auto & p : m_projectiles)
    {
        DrawableList::add(p, DrawableList::DrawHeight::PROJECTILE);
        Updatable::add(p);
    }
}

void ProjectileList::disable()
{
    m_enabled = false;

    for(const auto & p : m_projectiles)
    {
        DrawableList::del(p);
        Updatable::del(p);
    }
}

void ProjectileList::addProjectile(std::shared_ptr<Projectile> p)
{
    if(!p)
        return;
    std::shared_ptr<Room> r(p->getPos().getRoom());
    if(!r)
        return;
    if(r->getID() != m_playerRoom)
        return;

    if(std::find(m_projectiles.begin(), m_projectiles.end(), p) != m_projectiles.end())
        return;
    m_projectiles.push_back(p);

    if(m_enabled)
    {
        DrawableList::add(p, DrawableList::DrawHeight::PROJECTILE);
        Updatable::add(p);
    }
}

void ProjectileList::removeProjectile(std::shared_ptr<Projectile> p)
{
    if(!p)
        return;

    auto it = std::find(m_projectiles.begin(), m_projectiles.end(), p);
    if(it == m_projectiles.end())
        return;

    std::swap(*it, m_projectiles.back());
    m_projectiles.pop_back();
}

const std::vector<std::shared_ptr<Projectile> > & ProjectileList::projectiles() const
{
    return m_projectiles;
}

void ProjectileList::clean()
{
    auto it(std::remove_if(m_projectiles.begin(), m_projectiles.end(), [](const auto & p){if(!p) return true; return p->isKilled();}));
    m_projectiles.erase(it, m_projectiles.end());
}

void ProjectileList::clear()
{
    m_projectiles.clear();
}

void ProjectileList::onPlayerChangeRoom(EventPrePlayerChangeRoom e)
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

void ProjectileList::onProjectileCreated(EventProjectileCreated e)
{
    addProjectile(e.projectile);
}

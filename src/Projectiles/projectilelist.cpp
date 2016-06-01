#include "projectilelist.h"
#include "Systemes/drawablelist.h"
#include "Entities/entitylist.h"
#include "Map/room.h"
#include <algorithm>

ProjectileList ProjectileList::m_instance;

const unsigned int projectileHeight(1);

ProjectileList::ProjectileList()
    : m_playerRoom(0)
{
    connect<EventPreEntityChangeRoom>(std::bind(&ProjectileList::onPlayerChangeRoom, this, _1));
    connect<EventProjectileCreated>(std::bind(&ProjectileList::onProjectileCreated, this, _1));
}

ProjectileList & ProjectileList::list()
{
    return m_instance;
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

    if(std::find(m_projectiles.begin(), m_projectiles.end(), p) == m_projectiles.end())
        return;
    m_projectiles.push_back(p);

    DrawableList::add(p, projectileHeight);
    Updatable::add(p);
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

const std::vector<std::shared_ptr<Projectile>> ProjectileList::projectiles() const
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

void ProjectileList::onPlayerChangeRoom(EventPreEntityChangeRoom e)
{
    clear();
    std::shared_ptr<Entity> entity(EntityList::list().entity(e.entityID));
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

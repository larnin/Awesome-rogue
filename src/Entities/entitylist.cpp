#include <algorithm>
#include "entitylist.h"
#include "Map/room.h"
#include "Systemes/updatable.h"
#include "Collisions/pathfinder.h"
#include "Events/eventgetter.h"
#include "Systemes/drawablelist.h"

bool EntityList::m_instanced(false);

unsigned int entityHeight(2);

EntityList::EntityList()
    : m_currentRoom(0)
{
    assert(!m_instanced);
    m_instanced = true;

    connect<EventPrePlayerChangeRoom>(std::bind(&EntityList::onPlayerChangeRoom, this, _1));
    connect<EventEntityCreated>(std::bind(&EntityList::onEntityCreated, this, _1));
    connect<EventEntityChangeRoom>(std::bind(&EntityList::onEntityChangeRoom, this, _1));

    EventGetter<std::shared_ptr<Entity>,unsigned int>::connect(std::bind(&EntityList::entity, this, _1));
    EventGetter<std::vector<std::shared_ptr<Entity>>,unsigned int>::connect(std::bind(&EntityList::entitiesOn, this, _1));
}

EntityList::~EntityList()
{
    EventGetter<std::shared_ptr<Entity>,unsigned int>::disconnect();
    EventGetter<std::vector<std::shared_ptr<Entity>>,unsigned int>::disconnect();
}

void EntityList::addEntity(std::shared_ptr<Entity> entity)
{
    if(!entity)
        return;
    if(std::find(m_entities.begin(), m_entities.end(), entity) != m_entities.end())
        return;

    m_entities.push_back(entity);

    std::shared_ptr<Room> r(entity->getPos().getRoom().lock());
    if(!r)
        return;
    if(r->getID() != m_currentRoom)
        return;
        activeEntity(entity);
    DrawableList::add(entity, entityHeight);
}

void EntityList::removeEntity(std::shared_ptr<Entity> entity)
{
    if(!entity)
        return;

    auto it(std::find(m_entities.begin(), m_entities.end(), entity));
    if(it == m_entities.end())
        return;
    std::swap(*it, m_entities.back());
    m_entities.pop_back();

    disableEntity(entity);
}

std::vector<std::shared_ptr<Entity>> EntityList::entitiesOn(unsigned int room)
{
    std::vector<std::shared_ptr<Entity>> returnList;
    for(const auto & e : m_entities)
    {
        if(!e)
            continue;
        std::shared_ptr<Room> r(e->getPos().getRoom().lock());
        if(!r)
            continue;
        if(r->getID() == room)
            returnList.push_back(e);
    }
    return returnList;
}

void EntityList::clean()
{
    auto it(std::remove_if(m_entities.begin(), m_entities.end(), [](const auto & e){if(!e) return true; return e->isKilled(); }));
    m_entities.erase(it, m_entities.end());

    cleanActive();
}

void EntityList::clear()
{
    m_entities.clear();

    clearActive();
}

void EntityList::activeEntity(std::shared_ptr<Entity> e)
{
    if(!e)
        return;
    if(std::find(m_activeEntities.begin(), m_activeEntities.end(), e) != m_activeEntities.end())
        return;

    m_activeEntities.push_back(e);
    Updatable::add(e);
}

void EntityList::disableEntity(std::shared_ptr<Entity> e)
{
    if(!e)
        return;
    auto it(std::find(m_activeEntities.begin(), m_activeEntities.end(), e));
    if(it != m_activeEntities.end())
        std::swap(*it, m_activeEntities.back());
    m_activeEntities.pop_back();
    Updatable::del(e);
}

void EntityList::cleanActive()
{
    for(const auto & e : m_activeEntities)
        if(e->isKilled())
            Updatable::del(e);

    auto it(std::remove_if(m_activeEntities.begin(), m_activeEntities.end(), [](const auto & e){if(!e) return true; return e->isKilled(); }));
    m_activeEntities.erase(it, m_activeEntities.end());
}

void EntityList::clearActive()
{
    for(const auto & e : m_activeEntities)
        Updatable::del(e);
    m_activeEntities.clear();
}

void EntityList::onPlayerChangeRoom(EventPrePlayerChangeRoom e)
{
    std::shared_ptr<Entity>player(entity(e.entityID));
    if(!player)
        return;
    std::shared_ptr<Room> r(player->getPos().getRoom().lock());
    if(!r)
        return;

    m_currentRoom = r->getID();

    for(const auto & e : m_activeEntities)
        DrawableList::del(e);

    std::vector<std::shared_ptr<Entity>> list(entitiesOn(r->getID()));
    for(const auto & entity : list)
    {
        activeEntity(entity);
        DrawableList::add(entity, entityHeight);
    }

    do
    {
        auto it(std::find_if(m_activeEntities.begin(), m_activeEntities.end(), [r](const auto & entity)
        {
            if(!entity)
                return true;
            std::shared_ptr<Room> rE(entity->getPos().getRoom().lock());
            if(!r)
                return true;
            if(r == rE)
                return false;
            if(!entity->canPassDoor() || entity->getActiveDistance() < PathFinder::pathRooms(r, rE).size())
                return true;
            return false;
        }));
        if(it == m_activeEntities.end())
            break;
        disableEntity(*it);
    }while(true);
}

void EntityList::onEntityCreated(EventEntityCreated e)
{
    addEntity(e.entity);
}

void EntityList::onEntityChangeRoom(EventEntityChangeRoom e)
{
    std::shared_ptr<Entity> eLock(entity(e.entityID));
    if(!eLock)
        return;
    std::shared_ptr<Room>r(eLock->getPos().getRoom().lock());
    if(!r)
        return;
    if(r->getID() == m_currentRoom)
        DrawableList::add(eLock, entityHeight);
}

std::shared_ptr<Entity> EntityList::entity(unsigned int ID)
{
    auto it(std::find_if(m_entities.begin(), m_entities.end() , [ID](const auto & e)
    {
        if(!e)
            return false;
        return e->getID() == ID;
    }));
    if(it == m_entities.end())
        return std::shared_ptr<Entity>();
    return *it;
}
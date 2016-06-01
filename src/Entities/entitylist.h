#ifndef ENTITYLIST_H
#define ENTITYLIST_H

#include <vector>
#include <memory>
#include "Types/entity.h"
#include "Events/eventreceiver.h"
#include "Events/Datas/evententitycreated.h"
#include "Events/Datas/eventpreentitychangeroom.h"

class EntityList : public EventReceiver
{
public:
    virtual ~EntityList() = default;

    void addEntity(std::shared_ptr<Entity> entity);
    void removeEntity(std::shared_ptr<Entity> entity);
    std::shared_ptr<Entity> entity(unsigned int ID);

    std::vector<std::shared_ptr<Entity>> entitiesOn(unsigned int room);

    void clean();
    void clear();

    static EntityList& list();

private:
    EntityList();
    void activeEntity(std::shared_ptr<Entity> e);
    void disableEntity(std::shared_ptr<Entity> e);
    void cleanActive();
    void clearActive();

    void onPlayerChangeRoom(EventPreEntityChangeRoom e);
    void onEntityCreated(EventEntityCreated e);
    std::vector<std::shared_ptr<Entity>> m_entities;
    std::vector<std::shared_ptr<Entity>> m_activeEntities;

    static EntityList m_instance;
};

#endif // ENTITYLIST_H

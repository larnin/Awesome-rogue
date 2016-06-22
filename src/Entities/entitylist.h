#ifndef ENTITYLIST_H
#define ENTITYLIST_H

#include <vector>
#include <memory>
#include "Types/entity.h"
#include "Events/eventreceiver.h"
#include "Events/Datas/evententitycreated.h"
#include "Events/Datas/eventpreplayerchangeroom.h"
#include "Events/Datas/evententitychangeroom.h"
#include "Utilities/noncopiable.h"

class EntityList : public EventReceiver , private NonCopiable
{
public:
    EntityList();
    EntityList(EntityList &&) = default;
    EntityList & operator =(EntityList &&) = default;
    virtual ~EntityList();

    void addEntity(std::shared_ptr<Entity> entity);
    void removeEntity(std::shared_ptr<Entity> entity);
    std::shared_ptr<Entity> entity(unsigned int ID);

    std::vector<std::shared_ptr<Entity>> entitiesOn(unsigned int room);

    void clean();
    void clear();

private:
    void activeEntity(std::shared_ptr<Entity> e);
    void disableEntity(std::shared_ptr<Entity> e);
    void cleanActive();
    void clearActive();

    void onPlayerChangeRoom(EventPrePlayerChangeRoom e);
    void onEntityCreated(EventEntityCreated e);
    void onEntityChangeRoom(EventEntityChangeRoom e);

    std::vector<std::shared_ptr<Entity>> m_entities;
    std::vector<std::shared_ptr<Entity>> m_activeEntities;
    unsigned int m_currentRoom;

    static bool m_instanced;
};

#endif // ENTITYLIST_H

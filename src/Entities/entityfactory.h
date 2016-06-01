#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <memory>
#include "entitytype.h"
#include "Types/entity.h"
#include "Map/location.h"

class EntityFactory
{
public:
    static std::shared_ptr<Entity> create(EntityType type, const Location & pos, bool sendEvent = true);
private:
    EntityFactory() = delete;
};

#endif // ENTITYFACTORY_H

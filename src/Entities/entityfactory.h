#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <memory>
#include <vector>
#include "entitytype.h"

class Location;
class Entity;

class EntityFactory
{
public:
    static std::vector<std::shared_ptr<Entity>> create(EntityType type, const Location & pos, bool sendEvent = true);
private:
    static std::vector<std::shared_ptr<Entity>> createBoss1Parts(const Location & pos);
    EntityFactory() = delete;
};

#endif // ENTITYFACTORY_H

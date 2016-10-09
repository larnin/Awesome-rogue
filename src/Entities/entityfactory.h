#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "entitytype.h"
#include <memory>
#include <vector>

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

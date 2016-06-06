#ifndef PROJECTILEFACTORY_H
#define PROJECTILEFACTORY_H

#include "Types/projectile.h"
#include "Events/event.h"
#include "Events/Datas/eventprojectilecreated.h"
#include <memory>

class ProjectileFactory
{
public:
    template<typename T, typename... ARGS>
    static std::shared_ptr<Projectile> create(ARGS &&... args)
    {
        return std::make_shared<T>(std::forward<ARGS>(args)...);
    }

    template<typename T, typename... ARGS>
    static std::shared_ptr<Projectile> createSend(ARGS &&... args)
    {
        std::shared_ptr<Projectile> p = std::make_shared<T>(std::forward<ARGS>(args)...);
        Event<EventProjectileCreated>::send(EventProjectileCreated(p));
        return p;
    }

private:
    ProjectileFactory() = delete;
};

#endif // PROJECTILEFACTORY_H


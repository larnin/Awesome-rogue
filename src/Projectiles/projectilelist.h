#ifndef PROJECTILELIST_H
#define PROJECTILELIST_H

#include "Events/eventreceiver.h"
#include "Utilities/noncopiable.h"
#include <vector>

class EventProjectileCreated;
class EventPrePlayerChangeRoom;
class Projectile;

class ProjectileList : public EventReceiver , private NonCopiable
{
public:
    ProjectileList();
    ProjectileList(ProjectileList &&) = default;
    ProjectileList & operator =(ProjectileList &&) = default;
    virtual ~ProjectileList();

    void addProjectile(std::shared_ptr<Projectile> p);
    void removeProjectile(std::shared_ptr<Projectile> p);

    const std::vector<std::shared_ptr<Projectile>> & projectiles() const;

    void clean();
    void clear();

    void enable();
    void disable();

private:
    void onPlayerChangeRoom(EventPrePlayerChangeRoom e);
    void onProjectileCreated(EventProjectileCreated e);

    unsigned int m_playerRoom;

    std::vector<std::shared_ptr<Projectile>> m_projectiles;
    bool m_enabled;

    static bool m_instanced;
};

#endif // PROJECTILELIST_H

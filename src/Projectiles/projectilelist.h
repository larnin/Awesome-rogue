#ifndef PROJECTILELIST_H
#define PROJECTILELIST_H

#include <vector>
#include "Events/eventreceiver.h"
#include "Events/Datas/eventprojectilecreated.h"
#include "Events/Datas/eventpreentitychangeroom.h"

class ProjectileList : public EventReceiver
{
public:
    virtual ~ProjectileList() = default;

    void addProjectile(std::shared_ptr<Projectile> p);
    void removeProjectile(std::shared_ptr<Projectile> p);

    const std::vector<std::shared_ptr<Projectile>> projectiles() const;

    void clean();
    void clear();

    static ProjectileList & list();

private:
    ProjectileList();
    void onPlayerChangeRoom(EventPreEntityChangeRoom e);
    void onProjectileCreated(EventProjectileCreated e);

    unsigned int m_playerRoom;

    std::vector<std::shared_ptr<Projectile>> m_projectiles;

    static ProjectileList m_instance;
};

#endif // PROJECTILELIST_H

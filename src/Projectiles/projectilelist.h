#ifndef PROJECTILELIST_H
#define PROJECTILELIST_H

#include <vector>
#include "Events/eventreceiver.h"
#include "Events/Datas/eventprojectilecreated.h"
#include "Events/Datas/eventpreplayerchangeroom.h"

class ProjectileList : public EventReceiver
{
public:
    ProjectileList();
    virtual ~ProjectileList();

    ProjectileList(const ProjectileList &) = delete;
    ProjectileList & operator =(const ProjectileList &) = delete;

    void addProjectile(std::shared_ptr<Projectile> p);
    void removeProjectile(std::shared_ptr<Projectile> p);

    const std::vector<std::shared_ptr<Projectile>> projectiles() const;

    void clean();
    void clear();

private:
    void onPlayerChangeRoom(EventPrePlayerChangeRoom e);
    void onProjectileCreated(EventProjectileCreated e);

    unsigned int m_playerRoom;

    std::vector<std::shared_ptr<Projectile>> m_projectiles;

    static bool m_instanced;
};

#endif // PROJECTILELIST_H

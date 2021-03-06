#ifndef PROJECTILELAUNCHER_H
#define PROJECTILELAUNCHER_H

#include "Systemes/updatable.h"
#include "Systemes/controlable.h"
#include "Utilities/noncopiable.h"
#include <memory>

class Entity;

class ProjectileLauncher : public Updatable, public Controlable, private NonCopiable
{
public:
    ProjectileLauncher(std::weak_ptr<Entity> holder);
    ProjectileLauncher(ProjectileLauncher &&) = default;
    ProjectileLauncher & operator =(ProjectileLauncher &&) = default;
    virtual ~ProjectileLauncher() = default;

protected:
    std::weak_ptr<Entity> m_holder;
};

#endif // PROJECTILELAUNCHER_H

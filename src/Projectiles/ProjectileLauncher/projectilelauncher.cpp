#include "projectilelauncher.h"
#include "Entities/Types/entity.h"

ProjectileLauncher::ProjectileLauncher(std::weak_ptr<Entity> holder)
    : Controlable(ControlState::ACTIVE)
    , m_holder(holder)
{

}


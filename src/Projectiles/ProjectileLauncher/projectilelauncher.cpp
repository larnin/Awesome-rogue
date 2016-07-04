#include "projectilelauncher.h"

ProjectileLauncher::ProjectileLauncher(std::weak_ptr<Entity> holder)
    : Controlable(ControlState::ACTIVE)
    , m_holder(holder)
{

}


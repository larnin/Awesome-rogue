#include <memory>
#include "controlable.h"
#include "Controles/commandsvalue.h"

Controlable::Controlable(ControlState state)
    : m_state(state)
{

}

void Controlable::update(CommandsValue v)
{
    applyModifications();

    std::vector<std::shared_ptr<Controlable>> backControleList;
    std::vector<std::shared_ptr<Controlable>> activeControleList;
    for(auto & u : m_objects)
    {
        std::shared_ptr<Controlable> uLock(u.lock());
        if(!uLock)
            continue;
        if(uLock->getState() == ControlState::ACTIVE)
            activeControleList.push_back(uLock);
        if(uLock->getState() == ControlState::BACKGROUND)
            backControleList.push_back(uLock);
    }

    for(auto & uLock : activeControleList)
        uLock->control(v);
    for(auto & uLock : backControleList)
        uLock->control(v);

    clean();
}

void Controlable::changeActiveState(ControlState state)
{
    m_state = state;
}

Controlable::ControlState Controlable::getState() const
{
    return m_state;
}

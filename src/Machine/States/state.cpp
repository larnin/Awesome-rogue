#include "state.h"
#include "Machine/statemachine.h"
#include "pausestate.h"
#include <cassert>

State::State(std::weak_ptr<StateMachine> machine)
    : m_machine(machine)
{

}

bool isSubState(StateType type)
{
    switch (type)
    {
    case ST_OPTIONS:
    case ST_PAUSE:
        return true;
    default:
        return false;
    }
}

std::unique_ptr<State> makeState(StateType type, std::weak_ptr<StateMachine> m)
{
    switch (type)
    {
    case ST_PAUSE:
        return std::make_unique<PauseState>(m);
    default:
        assert(false);
    }
    return std::unique_ptr<State>();
}

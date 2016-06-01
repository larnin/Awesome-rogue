#include "state.h"
#include "Machine/statemachine.h"

State::State(std::weak_ptr<StateMachine> machine)
    : m_machine(machine)
{

}

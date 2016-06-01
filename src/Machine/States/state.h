#ifndef STATE_H
#define STATE_H

#include <memory>
#include "Utilities/noncopiable.h"

class StateMachine;

class State : private NonCopiable
{
public:
    State(State&&) = default;
    State & operator= (State &&) = default;
    virtual ~State() = default;

protected:
    State(std::weak_ptr<StateMachine> machine);
    std::weak_ptr<StateMachine> m_machine;
};

#endif // STATE_H

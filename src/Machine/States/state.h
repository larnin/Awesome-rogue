#ifndef STATE_H
#define STATE_H

#include <memory>
#include "Utilities/noncopiable.h"

class StateMachine;

class State : private NonCopiable
{
    friend class StateMachine;
public:
    State(State&&) = default;
    State & operator= (State &&) = default;
    virtual ~State() = default;

protected:
    virtual void enable(){}
    virtual void disable(){}

    State(std::weak_ptr<StateMachine> machine);
    std::weak_ptr<StateMachine> m_machine;
};

#endif // STATE_H

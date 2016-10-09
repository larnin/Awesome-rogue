#ifndef STATE_H
#define STATE_H

#include "Utilities/noncopiable.h"
#include <memory>

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

enum StateType
{
    ST_GAME,
    ST_MENU,
    ST_OPTIONS,
    ST_PAUSE,
};

bool isSubState(StateType type);
std::unique_ptr<State> makeState(StateType type, std::weak_ptr<StateMachine> m);

#endif // STATE_H

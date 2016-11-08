#ifndef LOADSTATE_H
#define LOADSTATE_H

#include "state.h"

class LoadState : public State
{
public:
    LoadState(std::weak_ptr<StateMachine> machine);
    LoadState(LoadState &&) = default;
    LoadState & operator =(LoadState &&) = default;
    virtual ~LoadState() = default;

protected:
    virtual void enable();
    virtual void disable();
};

#endif // LOADSTATE_H

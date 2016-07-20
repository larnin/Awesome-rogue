#ifndef CONTROLABLE_H
#define CONTROLABLE_H

#include "staticlist.h"

class CommandsValue;

class Controlable : protected StaticList<Controlable>
{
public:
    enum ControlState
    {
        BACKGROUND,
        UNACTIVE,
        ACTIVE
    };

    Controlable(ControlState state = ControlState::UNACTIVE);
    virtual ~Controlable() = default;
    virtual void control(CommandsValue & v) = 0;
    virtual void changeActiveState(ControlState state);

    ControlState getState() const;

    static void update(CommandsValue v);

    using StaticList<Controlable>::add;
    using StaticList<Controlable>::del;
    using StaticList<Controlable>::clean;

protected:
    ControlState m_state;
};

#endif // CONTROLABLE_H

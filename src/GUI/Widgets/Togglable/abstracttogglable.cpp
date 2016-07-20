#include "abstracttogglable.h"
#include "Controles/commandsvalue.h"

AbstractTogglable::AbstractTogglable(const AdaptableBounds & _bounds, bool _state)
    : Widget(_bounds)
    , state(_state)
{

}

void AbstractTogglable::control(CommandsValue & v)
{
    if(v.isPressEvent(KEY_VALID))
    {
        state = !state;
        if(m_toggledFunction)
            m_toggledFunction(state);
    }
    Widget::control(v);
}

void AbstractTogglable::connecToggledEvent(const std::function<void(bool)> & function)
{
    m_toggledFunction = function;
}

void AbstractTogglable::disconnectToggledEvent()
{
    connecToggledEvent(std::function<void(bool)>());
}

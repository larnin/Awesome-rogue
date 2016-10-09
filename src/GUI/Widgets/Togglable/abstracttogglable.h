#ifndef ABSTRACTTOGGLABLE_H
#define ABSTRACTTOGGLABLE_H

#include "GUI/Widgets/widget.h"
#include <functional>

class AbstractTogglable : public Widget
{
public:
    AbstractTogglable(const AdaptableBounds & _bounds = AdaptableBounds(), bool _state = false);

    virtual ~AbstractTogglable() = default;

    virtual void control(CommandsValue & v);

    void connecToggledEvent(const std::function<void(bool)> & function);
    void disconnectToggledEvent();

    bool state;

private:
    std::function<void(bool)> m_toggledFunction;
};

#endif // ABSTRACTTOGGLABLE_H

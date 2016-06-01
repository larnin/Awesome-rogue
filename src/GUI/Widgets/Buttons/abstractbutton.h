#ifndef ABSTRACTBUTTON_H
#define ABSTRACTBUTTON_H

#include <functional>
#include <SFML/System/Time.hpp>
#include "GUI/Widgets/widget.h"

class AbstractButton : public Widget
{
public:
    AbstractButton(const AdaptableBounds & bounds = AdaptableBounds());
    virtual ~AbstractButton() = default;

    virtual void control(CommandsValue & v);
    virtual void update(const sf::Time & t);

    void connectClickEvent(const std::function<void(void)> & function);
    void disconnectClickEvent();

protected:
    sf::Time m_timeFromLastClick;

private:
    std::function<void(void)> m_clickFunction;

};

#endif // ABSTRACTBUTTON_H

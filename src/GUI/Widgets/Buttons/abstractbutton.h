#ifndef ABSTRACTBUTTON_H
#define ABSTRACTBUTTON_H

#include "GUI/Widgets/widget.h"
#include <SFML/System/Time.hpp>
#include <functional>

class AbstractButton : public Widget
{
public:
    AbstractButton(const AdaptableBounds & bounds = AdaptableBounds());
    virtual ~AbstractButton() = default;

    virtual void control(CommandsValue & v) override;
    virtual void update(const sf::Time & t) override;

    void connectClickEvent(const std::function<void(void)> & function);
    void disconnectClickEvent();

protected:
    sf::Time m_timeFromLastClick;

private:
    std::function<void(void)> m_clickFunction;

};

#endif // ABSTRACTBUTTON_H

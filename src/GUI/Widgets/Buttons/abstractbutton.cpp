#include "abstractbutton.h"
#include "Controles/commandsvalue.h"

AbstractButton::AbstractButton(const AdaptableBounds & bounds)
    : Widget(bounds)
{

}

void AbstractButton::control(CommandsValue & v)
{
    if(v.isPressEvent(KEY_VALID))
    {
        v.valide(KEY_VALID);
        if(m_clickFunction)
            m_clickFunction();
        m_timeFromLastClick = sf::Time();
    }
    Widget::control(v);
}

void AbstractButton::update(const sf::Time & t)
{
    Widget::update(t);
    m_timeFromLastClick += t;
}

void AbstractButton::connectClickEvent(const std::function<void(void)> & function)
{
    m_clickFunction = function;
}

void AbstractButton::disconnectClickEvent()
{
    connectClickEvent(std::function<void(void)>());
}

#include "widget.h"
#include "Controles/commandsvalue.h"

Widget::Widget(const AdaptableBounds & _bounds)
    : bounds(_bounds)
{

}

void Widget::connect(CommandType type, std::weak_ptr<Widget> widget)
{
    auto it(m_connexions.find(type));
    if(it == m_connexions.end())
        m_connexions.emplace(type, widget);
    else it->second = widget;
}

void Widget::disconnect(CommandType type)
{
    auto it(m_connexions.find(type));
    if(it != m_connexions.end())
        m_connexions.erase(it);
}

void Widget::control(CommandsValue & v)
{
    for(auto & connection : m_connexions)
    {
        if(v.isPressEvent(connection.first))
        {
            v.valide(connection.first);
            std::shared_ptr<Widget> w(connection.second.lock());
            if(w)
            {
                w->changeActiveState(Controlable::ControlState::ACTIVE);
                changeActiveState(Controlable::ControlState::UNACTIVE);
            }
            break;
        }
    }
}

void Widget::update(const sf::Time &)
{

}

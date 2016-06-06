#include "widget.h"

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

void Widget::control(CommandsValue & v)
{
    for(auto & connection : m_connexions)
    {
        if(v.isPressEvent(connection.first))
        {
            v.valide(connection.first);
            std::shared_ptr<Widget> w(connection.second);
            if(w)
                w->changeActiveState(Controlable::ControlState::ACTIVE);
            changeActiveState(Controlable::ControlState::UNACTIVE);
        }
    }
}

void Widget::update(const sf::Time &)
{

}
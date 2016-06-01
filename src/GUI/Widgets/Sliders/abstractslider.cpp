#include "abstractslider.h"

AbstractSlider::AbstractSlider(const AdaptableBounds & _bounds, bool vertical)
    : Widget(_bounds)
    , min(0.0f)
    , max(100.0f)
    , speed(20.0f)
    , value(0.0f)
    , m_vertical(vertical)
    , m_commandValue(0.0f)
{

}

void AbstractSlider::control(CommandsValue & v)
{
    if(m_vertical)
    {
        m_commandValue = v.getValue(MOVE_DOWN)-v.getValue(MOVE_UP);
        v.valide(MOVE_UP);
        v.valide(MOVE_DOWN);
    }
    else
    {
        m_commandValue = v.getValue(MOVE_RIGHT)-v.getValue(MOVE_LEFT);
        v.valide(MOVE_RIGHT);
        v.valide(MOVE_LEFT);
    }
    Widget::control(v);
}

void AbstractSlider::update(const sf::Time & t)
{
    Widget::update(t);
    if(m_state == ControlState::UNACTIVE)
        m_commandValue = 0;

    value += m_commandValue*t.asSeconds()*speed;
    value = std::min(max, std::max(min, value));
}

void AbstractSlider::connectValueChangedEvent(const std::function<void(float)> & function)
{
    m_valueChangedFunction = function;
}

void AbstractSlider::disconnectValueChangedEvent()
{
    connectValueChangedEvent(std::function<void(float)>());
}

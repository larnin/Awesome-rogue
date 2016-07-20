#include "abstractquantifiedslider.h"
#include "Controles/commandsvalue.h"

AbstractQuantifiedSlider::AbstractQuantifiedSlider(const AdaptableBounds & _bounds, bool vertical)
    : Widget(_bounds)
    , m_index(-1)
    , m_vertical(vertical)
{

}

void AbstractQuantifiedSlider::control(CommandsValue & v)
{
    if(!m_items.empty())
    {
        int move(0);
        if((v.isPressEvent(MOVE_LEFT) && !m_vertical) || (v.isPressEvent(MOVE_UP) && m_vertical))
        {
            move = -1;
            v.valide(m_vertical ? MOVE_UP : MOVE_LEFT);
        }
        if((v.isPressEvent(MOVE_RIGHT) && !m_vertical) || (v.isPressEvent(MOVE_DOWN) && m_vertical))
        {
            move = 1;
            v.valide(m_vertical ? MOVE_DOWN : MOVE_RIGHT);
        }
        m_index += move;
        if(m_index < 0)
            m_index = 0;
        if(m_index >= int(m_items.size()))
            m_index = m_items.size()-1;
    }
    Widget::control(v);
}

void AbstractQuantifiedSlider::connectValueChangedEvent(const std::function<void(std::string)> & function)
{
    m_valueChangedfunction = function;
}

void AbstractQuantifiedSlider::disconnectValueChangedEvent()
{
    connectValueChangedEvent(std::function<void(std::string)>());
}

void AbstractQuantifiedSlider::addItem(const std::string & item)
{
    m_items.push_back(item);
    if(m_index < 0)
        m_index = 0;
}

void AbstractQuantifiedSlider::clear()
{
    m_items.clear();
    m_index = -1;
}

void AbstractQuantifiedSlider::setCurrentIndex(int index)
{
    if(m_items.empty())
        index = -1;
    else if(index < 0)
        index = 0;
    else if(index >= int(m_items.size()))
        index = m_items.size()-1;
    m_index = index;
}

std::string AbstractQuantifiedSlider::current() const
{
    return at(m_index);
}

int AbstractQuantifiedSlider::currentIndex() const
{
    return m_index;
}

std::string AbstractQuantifiedSlider::at(int index) const
{
    if(index >= int(m_items.size()) || index < 0)
        return std::string();
    return m_items[index];
}

const std::vector<std::string> & AbstractQuantifiedSlider::items() const
{
    return m_items;
}

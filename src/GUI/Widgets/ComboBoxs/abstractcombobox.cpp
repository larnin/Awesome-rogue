#include "abstractcombobox.h"

AbstractComboBox::AbstractComboBox(const AdaptableBounds & _bounds)
    : Widget(_bounds)
    , m_index(-1)
    , m_isOpened(false)
    , m_currentIndex(-1)
{

}

void AbstractComboBox::control(CommandsValue & v)
{
    if(!m_isOpened)
    {
        if(v.isPressEvent(KEY_VALID) && !m_items.empty())
        {
            m_isOpened = true;
            v.valide(KEY_VALID);
            m_currentIndex = m_index;
        }
    }
    else
    {
        if(!m_items.empty())
        {
            if(v.isPressEvent(MOVE_DOWN))
            {
                v.valide(MOVE_DOWN);
                if(m_currentIndex < int(m_items.size())-1)
                    m_currentIndex++;
            }
            if(v.isPressEvent(MOVE_UP))
            {
                v.valide(MOVE_UP);
                if(m_currentIndex > 0)
                    m_currentIndex--;
            }
        }

        if(v.isPressEvent(KEY_VALID))
        {
            m_isOpened = false;
            v.valide(KEY_VALID);
            m_index = m_currentIndex;
            if(m_itemSelectedFunction && m_index < int(m_items.size()) && m_index >= 0)
                m_itemSelectedFunction(m_items[m_index]);
        }
    }

    Widget::control(v);
}

void AbstractComboBox::connectItemSelectedEvent(const std::function<void(std::string)> & function)
{
    m_itemSelectedFunction = function;
}

void AbstractComboBox::disconnectItemSelectedEvent()
{
    connectItemSelectedEvent(std::function<void(std::string)>());
}

void AbstractComboBox::addItem(const std::string & item)
{
    m_items.push_back(item);
    if(m_index < 0)
        m_index = 0;
}

void AbstractComboBox::clear()
{
    m_items.clear();
    m_index = -1;
}

void AbstractComboBox::setCurrentIndex(int index)
{
    if(m_items.empty())
        index = -1;
    else if(index < 0)
        index = 0;
    else if(index >= int(m_items.size()))
        index = m_items.size()-1;
    m_index = index;
}

std::string AbstractComboBox::current() const
{
    return at(m_index);
}

int AbstractComboBox::currentIndex() const
{
    return m_index;
}

std::string AbstractComboBox::at(int index) const
{
    if(index < 0 || index >= int(m_items.size()))
        return std::string();
    return m_items[index];
}

const std::vector<std::string> & AbstractComboBox::items() const
{
    return m_items;
}

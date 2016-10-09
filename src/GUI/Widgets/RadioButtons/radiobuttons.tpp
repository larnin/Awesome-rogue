#include "Controles/commandsvalue.h"
#include <algorithm>
#include <SFML/Graphics/RenderTarget.hpp>

//#include "radiobuttons.h"

template <typename T>
RadioButtons<T>::RadioButtons(AdaptableBounds bounds , bool vertical)
    : Widget(bounds)
    , m_vertical(vertical)
{

}

template <typename T>
void RadioButtons<T>::control(CommandsValue & v)
{
    if(!m_buttons.empty())
    {
        replaceCurrent();
        bool canUp(m_currentIndex > 0);
        bool canDown(m_currentIndex < int(m_buttons.size())-1);
        int move(0);

        if(((v.isPressEvent(MOVE_LEFT) && !m_vertical) || (v.isPressEvent(MOVE_UP) && m_vertical)) && canUp)
        {
            move = -1;
            v.valide(m_vertical ? MOVE_UP : MOVE_LEFT);
        }
        if(((v.isPressEvent(MOVE_RIGHT) && !m_vertical) || (v.isPressEvent(MOVE_DOWN) && m_vertical)) && canDown)
        {
            move = 1;
            v.valide(m_vertical ? MOVE_DOWN : MOVE_RIGHT);
        }
        if(move != 0)
        {
            for(auto & item : m_buttons)
                item.selected = false;
            m_currentIndex += move;
            if(m_currentIndex < 0)
                m_currentIndex = 0;
            if(m_currentIndex >= int(m_buttons.size()))
                m_currentIndex = m_buttons.size()-1;
            m_buttons[m_currentIndex].selected = true;
        }
        if(v.isPressEvent(KEY_VALID))
        {
            v.valide(KEY_VALID);
            for(auto & item : m_buttons)
                item.checked = false;
            m_buttons[m_currentIndex].checked = true;
            if(m_indexChangedFunction)
                m_indexChangedFunction(m_buttons[m_currentIndex].label);
        }
    }
    Widget::control(v);
}

template <typename T>
void RadioButtons<T>::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    if(m_buttons.empty())
        return;

    sf::FloatRect rect(bounds.boundsOnView(target.getView()));
    sf::Vector2f buttonSize;
    sf::Vector2f offset;
    if(m_vertical)
    {
        buttonSize = sf::Vector2f(rect.width, rect.height/m_buttons.size());
        offset = sf::Vector2f(0, buttonSize.y);
    }
    else
    {
        buttonSize = sf::Vector2f(rect.width/m_buttons.size(), rect.height);
        offset = sf::Vector2f(buttonSize.x, 0);
    }
    for(unsigned int i(0) ; i < m_buttons.size() ; i++)
    {
        sf::FloatRect buttonRect(sf::Vector2f(rect.left, rect.top) + offset*float(i), buttonSize);
        m_buttons[i].draw(target, states, buttonRect);
    }
}

template <typename T>
void RadioButtons<T>::changeActiveState(ControlState state)
{
    if(state == ControlState::UNACTIVE)
        for(auto & item : m_buttons)
            item.selected = false;
    else if(state == ControlState::ACTIVE || state == ControlState::BACKGROUND)
    {
        if(!m_buttons.empty())
        {
            m_currentIndex = 0;
            for(unsigned int i(0) ; i < m_buttons.size() ; i++)
                if(m_buttons[i].checked)
                    m_currentIndex = i;

            m_buttons[m_currentIndex].selected = true;
        }
    }
    Widget::changeActiveState(state);
}

template <typename T>
void RadioButtons<T>::clear()
{
    m_buttons.clear();
}

template <typename T>
void RadioButtons<T>::addButton(const std::string & label)
{
    m_buttons.emplace_back(T());
    m_buttons.back().label = label;
}

template <typename T>
void RadioButtons<T>::setCurrent(const std::string & label)
{
    auto it(std::find_if(m_buttons.begin(), m_buttons.end(), [label](const auto & b){return b.label == label;}));
    if(it != m_buttons.end())
    {
        for(auto & item : m_buttons)
            item.checked = false;
        it->checked = true;
    }
}

template <typename T>
std::string RadioButtons<T>::getCurrent() const
{
    for(const auto & item : m_buttons)
        if(item.checked)
            return item.label;
    return "";
}

template <typename T>
const std::vector<std::string> & RadioButtons<T>::allLabels() const
{
    std::vector<std::string> labels;

    for(const auto & item : m_buttons)
        labels.push_back(item.label);
    return labels;
}

template <typename T>
void RadioButtons<T>::connectIndexChangedEvent(const std::function<void(std::string)> & function)
{
    m_indexChangedFunction = function;
}

template <typename T>
void RadioButtons<T>::disconnectIndexChangedEvent()
{
    connectIndexChangedEvent(std::function<void(std::string)>());
}

template <typename T>
void RadioButtons<T>::replaceCurrent()
{
    if(m_buttons.empty())
        return;
    if(m_currentIndex < 0)
        m_currentIndex = 0;
    if(m_currentIndex >= int(m_buttons.size()))
        m_currentIndex = m_buttons.size()-1;
}

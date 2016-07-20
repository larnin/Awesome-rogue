#include "abstracttextbox.h"
#include <SFML/Window/Event.hpp>
#include "Controles/commandsvalue.h"

AbstractTextBox::AbstractTextBox(const AdaptableBounds & _bounds)
    : Widget(_bounds)
    , m_cursorIndex(0)
{

}

void AbstractTextBox::control(CommandsValue & v)
{
    replaceCursor();
    for(const sf::Event & e : v.events)
    {
        if(e.type == sf::Event::TextEntered)
        {
            if(e.text.unicode < 256 && e.text.unicode > 10)
            {
                text.insert(m_cursorIndex, 1, e.text.unicode);
                if(m_textEntredFunction)
                    m_textEntredFunction(text);
                m_cursorIndex++;
            }
        }
        else if(e.type == sf::Event::KeyPressed)
        {
            if(e.key.code == sf::Keyboard::BackSpace)
            {
                if(!text.empty() && m_cursorIndex > 0)
                {
                    text.erase(m_cursorIndex-1, 1);
                    m_cursorIndex--;
                }
            }
            else if(e.key.code == sf::Keyboard::Left && m_cursorIndex > 0)
                m_cursorIndex--;
            else if(e.key.code == sf::Keyboard::Right && m_cursorIndex < text.size())
                m_cursorIndex++;
        }
    }
    Widget::control(v);
}

void AbstractTextBox::connecTextEntredEvent(const std::function<void(std::string)> & function)
{
    m_textEntredFunction = function;
}

void AbstractTextBox::disconnectTextEntredEvent()
{
    connecTextEntredEvent(std::function<void(std::string)>());
}

void AbstractTextBox::replaceCursor()
{
    if(m_cursorIndex > text.size())
        m_cursorIndex = text.size();
}

#include "basictextbox.h"
#include "GUI/Widgets/graphichelper.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

BasicTextBox::BasicTextBox(const AdaptableBounds & _bounds)
    : AbstractTextBox(_bounds)
    , m_texture("res/img/gui/textbox.png")
    , m_font("res/font/PressStart2P.ttf")
{

}

void BasicTextBox::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::FloatRect b(bounds.boundsOnView(target.getView()));

    if(m_state == ControlState::ACTIVE)
        target.draw(drawSelector(b));

    target.draw(drawButton(b, sf::FloatRect(0, 0, 11, 11), Margin(5)), sf::RenderStates(m_texture()));

    updateTextTexture(sf::Vector2f(b.width-15, b.height-6), (m_cursorTimer.asMilliseconds()/500)%2 ==0 && m_state == ControlState::ACTIVE);
    const sf::Vector2f size(m_textTexture.getSize());
    sf::Sprite s(m_textTexture.getTexture());
    s.setPosition(b.left + (b.width-size.x)/2.0f, b.top + (b.height-size.y)/2.0f);
    target.draw(s);
}

void BasicTextBox::update(const sf::Time & t)
{
    AbstractTextBox::update(t);
    m_cursorTimer+=t;
}

void BasicTextBox::updateTextTexture(const sf::Vector2f & size, bool drawCursor) const
{
    if(size.x < 1 || size.y < 1)
        return;

    if((unsigned int)(size.x) != m_textTexture.getSize().x-3 || (unsigned int)(size.y) != m_textTexture.getSize().y)
        m_textTexture.create(size.x+3, size.y);

    sf::Text t(text, *m_font, 8);
    t.setColor(sf::Color::Black);
    sf::Vector2f pos(-t.getGlobalBounds().left, -t.getGlobalBounds().top + int((size.y-t.getGlobalBounds().height)/2.0f));
    if(t.getGlobalBounds().width > size.x)
        pos.x -= t.getGlobalBounds().width - size.x;
    t.setPosition(pos);
    float cursorPos(t.findCharacterPos(m_cursorIndex).x);
    float leftViewPos(m_cursorIndex == 0 ? cursorPos : t.findCharacterPos(m_cursorIndex-1).x);
    if(leftViewPos < 0)
    {
        cursorPos -= leftViewPos;
        t.move(-leftViewPos, 0);
    }

    m_textTexture.clear(sf::Color(0, 0, 0, 0));

    m_textTexture.draw(t);
    if(drawCursor)
    {
        const float cursorHeight(9);
        sf::RectangleShape cursor(sf::Vector2f(1, cursorHeight));
        cursor.setFillColor(sf::Color::Black);
        cursor.setPosition(cursorPos, (m_textTexture.getSize().y-cursorHeight)/2.0f);
        m_textTexture.draw(cursor);
    }

    m_textTexture.display();
}

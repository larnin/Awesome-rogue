#include "togglablebasicbutton.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "GUI/Widgets/graphichelper.h"

TogglableBasicButton::TogglableBasicButton(const AdaptableBounds & _bounds, const std::string & text)
    : AbstractTogglable(_bounds)
    , m_texture("res/img/gui/button.png")
    , m_font("res/font/PressStart2P.ttf")
    , m_text(VAlign::V_CENTER, HAlign::H_CENTER)
{
    m_text.setText(text, m_font, 8, sf::Color::Black);
}

void TogglableBasicButton::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::FloatRect b(bounds.boundsOnView(target.getView()));

    if(m_state == ControlState::ACTIVE)
        target.draw(drawSelector(b));

    float offset(state ? 12 : 0);

    target.draw(drawButton(b, sf::FloatRect(offset, 0, 11, 11), Margin(5)), sf::RenderStates(m_texture()));

    sf::FloatRect textRect(Margin::applyMargin(b, 4));
    if(state)
    {
        textRect.left++;
        textRect.top++;
    }
    m_text.draw(target, textRect);
}

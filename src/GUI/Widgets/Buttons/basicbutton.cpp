#include "basicbutton.h"
#include "Utilities/quadrender.h"
#include <GUI/Widgets/graphichelper.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

BasicButton::BasicButton(const AdaptableBounds & _bounds, const std::string & text)
    :AbstractButton(_bounds)
    , m_texture("res/img/gui/button.png")
    , m_font("res/font/PressStart2P.ttf")
    , m_text(VAlign::V_CENTER, HAlign::H_CENTER)
{
    m_text.setText(text, m_font, 8, sf::Color::Black);
}

void BasicButton::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::FloatRect b(bounds.boundsOnView(target.getView()));

    if(m_state == ControlState::ACTIVE)
        target.draw(drawSelector(b));

    const float maxPressTime(200);
    bool pressed(m_timeFromLastClick.asMilliseconds()<maxPressTime);
    float offset(pressed ? 12 : 0);

    target.draw(drawButton(b, sf::FloatRect(offset, 0, 11, 11), Margin(5)), sf::RenderStates(m_texture()));

    sf::FloatRect textBounds(Margin::applyMargin(b, Margin(4)));
    if(pressed)
    {
        textBounds.left++;
        textBounds.top++;
    }
    m_text.draw(target, textBounds);
}

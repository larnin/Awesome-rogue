#include "basicslider.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include "GUI/Widgets/graphichelper.h"
#include "Utilities/quadrender.h"

BasicSlider::BasicSlider(const AdaptableBounds & _bounds)
    : AbstractSlider(_bounds)
    , m_texture("res/img/gui/slider.png")
    , m_font("res/font/PressStart2P.ttf")
    , m_text(VAlign::V_CENTER, HAlign::H_RIGHT)
{
    m_text.setFont(m_font);
    m_text.setTextColor(sf::Color::Black);
    m_text.setTextSize(8);
}

void BasicSlider::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::FloatRect b(bounds.boundsOnView(target.getView()));

    if(m_state == ControlState::ACTIVE)
        target.draw(drawSelector(b));

    const float barHeight(9);
    const float textWidth(30);
    const float cursorWidth(10);
    const float cursorHeight(18);
    sf::FloatRect barBounds(b.left, b.top+(b.height-barHeight)/2.0f, b.width-textWidth, barHeight);

    target.draw(drawButton(barBounds, sf::FloatRect(0, 0, 9, 9), Margin(3, 4, 3, 4)), sf::RenderStates(m_texture()));
    sf::VertexArray buttonArray(sf::Quads, 4);
    drawQuad(&buttonArray[0], sf::FloatRect((value-min)/(max-min)*(b.width-textWidth-cursorWidth)+b.left, b.top+(b.height-cursorHeight)/2.0f, cursorWidth, cursorHeight)
            , sf::FloatRect(0, 9, cursorWidth, cursorHeight));
    target.draw(buttonArray, sf::RenderStates(m_texture()));

    m_text.setText(std::to_string(int(value)));
    m_text.draw(target, sf::FloatRect(b.left+b.width-textWidth, b.top, textWidth-2, b.height));
}

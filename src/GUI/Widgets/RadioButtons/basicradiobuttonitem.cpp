#include "basicradiobuttonitem.h"
#include "GUI/Widgets/graphichelper.h"
#include <SFML/Graphics/VertexArray.hpp>
#include "Utilities/quadrender.h"

BasicRadioButtonItem::BasicRadioButtonItem()
    : AbstactRadioButtonItem()
    , m_texture("res/img/gui/radiobutton.png")
    , m_font("res/font/PressStart2P.ttf")
    , m_text(VAlign::V_CENTER, HAlign::H_RIGHT)
{
    m_text.setText(label, m_font, 8, sf::Color::Black);
}

void BasicRadioButtonItem::draw(sf::RenderTarget & target, sf::RenderStates, const sf::FloatRect & bounds) const
{
    if(m_text.getText() != label)
        m_text.setText(label);

    if(selected)
        target.draw(drawSelector(bounds));

    float offset(checked ? 18 : 0);

    sf::VertexArray array(sf::Quads, 4);
    drawQuad(&array[0], sf::FloatRect(bounds.left, bounds.top + (bounds.height-18)/2.0f, 18, 18), sf::FloatRect(offset, 0, 18, 18));
    target.draw(array, sf::RenderStates(m_texture()));

    m_text.draw(target, sf::FloatRect(bounds.left+20, bounds.top, bounds.width-22, bounds.height));
}

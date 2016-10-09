#include "basiccheckbox.h"
#include "GUI/Widgets/graphichelper.h"
#include "Utilities/quadrender.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

BasicCheckbox::BasicCheckbox(const AdaptableBounds & _bounds, const std::string & label)
    : AbstractTogglable(_bounds)
    , m_texture("res/img/gui/checkbox.png")
    , m_font("res/font/PressStart2P.ttf")
    , m_text(VAlign::V_CENTER, HAlign::H_RIGHT)
{

    m_text.setText(label, m_font, 8, sf::Color::Black);
}

void BasicCheckbox::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::FloatRect b(bounds.boundsOnView(target.getView()));

    if(m_state == ControlState::ACTIVE)
        target.draw(drawSelector(b));

    float offset(state ? 20 : 0);

    sf::VertexArray array(sf::Quads, 4);
    drawQuad(&array[0], sf::FloatRect(b.left, b.top + (b.height-20)/2.0f, 20, 20), sf::FloatRect(offset, 0, 20, 20));
    target.draw(array, sf::RenderStates(m_texture()));

    m_text.draw(target, sf::FloatRect(b.left+20, b.top, b.width-22, b.height));
}

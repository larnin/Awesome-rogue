#include "basicquantifiedslider.h"
#include "Utilities/quadrender.h"
#include "GUI/Widgets/graphichelper.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Sprite.hpp>

BasicQuantifiedSlider::BasicQuantifiedSlider(const AdaptableBounds & _bounds)
    : AbstractQuantifiedSlider(_bounds)
    , m_texture("res/img/gui/quantifiedslider.png")
    , m_font("res/font/PressStart2P.ttf")
{

}

void BasicQuantifiedSlider::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    updateLabels();

    sf::FloatRect b(bounds.boundsOnView(target.getView()));

    if(m_state == ControlState::ACTIVE)
        target.draw(drawSelector(b));

    sf::VertexArray array(sf::Quads, 16+4*m_items.size());
    drawQuad(&array[0], sf::FloatRect(b.left, b.top+b.height-11, 3, 9), sf::FloatRect(0, 0, 3, 9));
    drawQuad(&array[4], sf::FloatRect(b.left+b.width-3, b.top+b.height-11, 3, 9), sf::FloatRect(5, 0, 3, 9));
    drawQuad(&array[8], sf::FloatRect(b.left+3, b.top+b.height-11, b.width-6, 9), sf::FloatRect(3, 0, 2, 9));


    if(!m_items.empty())
    {
        const float offset(9);
        const float totalWidth(b.width-2*offset);
        float itemDelta(totalWidth);
        if(m_items.size() > 1)
            itemDelta/= m_items.size()-1;
        for(unsigned int i(0) ; i < m_items.size() ; i++)
        {
            drawQuad(&array[12+4*i], sf::FloatRect(b.left+offset+i*itemDelta-5, b.top+b.height-13, 10, 13)
                    , sf::FloatRect(17, 0, 10, 13));

            m_texts[i].draw(target, sf::FloatRect(b.left+offset+(i-0.5f)*itemDelta, b.top+b.height-35, itemDelta, 16));

            if(int(i) == m_index)
                drawQuad(&array[array.getVertexCount()-4], sf::FloatRect(b.left+offset+i*itemDelta-3, b.top+b.height-16, 6, 18)
                        , sf::FloatRect(10, 0, 6, 18));
        }
    }

    target.draw(array, sf::RenderStates(m_texture()));
}

void BasicQuantifiedSlider::updateLabels() const
{
    while(m_texts.size() < m_items.size())
    {
        m_texts.emplace_back(VAlign::V_BOTTOM, HAlign::H_CENTER);
        m_texts.back().setFont(m_font);
        m_texts.back().setTextColor(sf::Color::Black);
        m_texts.back().setTextSize(8);
    }
    while(m_texts.size() > m_items.size())
        m_texts.pop_back();

    for(unsigned int i(0) ; i < m_items.size() ; i++)
        if(m_texts[i].getText() != m_items[i])
            m_texts[i].setText(m_items[i]);
}

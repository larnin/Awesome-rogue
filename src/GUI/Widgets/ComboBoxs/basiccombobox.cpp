#include "basiccombobox.h"
#include "GUI/Widgets/graphichelper.h"
#include "Utilities/quadrender.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

BasicCombobox::BasicCombobox(const AdaptableBounds & _bounds)
    : AbstractComboBox(_bounds)
    , m_texture("res/img/gui/combobox.png")
    , m_font("res/font/PressStart2P.ttf")
    , m_label(VAlign::V_CENTER, HAlign::H_LEFT)
{
    m_label.setFont(m_font);
    m_label.setTextColor(sf::Color::Black);
    m_label.setTextSize(8);
}

void BasicCombobox::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    updateTexts();

    sf::FloatRect b(bounds.boundsOnView(target.getView()));

    if(m_state == ControlState::ACTIVE)
        target.draw(drawSelector(b));

    float offsetTexture(m_isOpened ? 23 : 0);
    target.draw(drawButton(b, sf::FloatRect(offsetTexture, 0, 23, 13), Margin(7, 16, 7, 7)), sf::RenderStates(m_texture()));

    sf::Vector2f offset(m_isOpened ? 1 : 0, m_isOpened ? 1 : 0);

    sf::VertexArray arrayArrow(sf::Quads, 4);
    drawQuad(&arrayArrow[0], sf::FloatRect(b.left + b.width-13 + offset.x, b.top + (b.height-7)/2, 7, 6)
            , sf::FloatRect(0, 14, 7, 6), false, m_isOpened);
    target.draw(arrayArrow, m_texture());

    sf::FloatRect textBounds(Margin::applyMargin(b, Margin(6, 17, 6, 6)));
    if(m_isOpened)
    {
        textBounds.left++;
        textBounds.top++;
    }
    m_label.draw(target, textBounds);

    if(m_isOpened)
    {
        int minIndex(m_currentIndex-2);
        int maxIndex(m_currentIndex+2);
        if(minIndex < 0)
        {
            maxIndex -= minIndex;
            minIndex = 0;
        }
        if(maxIndex >= int(m_items.size()))
        {
            minIndex -= (maxIndex-m_items.size());
            maxIndex = m_items.size()-1;
        }
        minIndex = std::max(0, minIndex);
        const float caseHeight(16);
        const float caseWidth(b.width-7);
        for(int i(0) ; i <= maxIndex-minIndex ; i++)
        {
            float offset((minIndex+i)%2 == 0 ? 8 : 16);
            if(minIndex+i == m_currentIndex)
                offset = 24;

            sf::FloatRect itemRect(b.left+4, b.top + b.height +(caseHeight-1)*i-1, caseWidth, caseHeight);
            target.draw(drawButton(itemRect, sf::FloatRect(offset, 14, 7, 7), Margin(3)), sf::RenderStates(m_texture()));

            m_texts[i+minIndex].draw(target, Margin::applyMargin(itemRect, Margin(3)));
        }
    }
}

void BasicCombobox::updateTexts() const
{
    if(m_label.getText() != current())
        m_label.setText(current());

    while(m_texts.size() < m_items.size())
    {
        m_texts.emplace_back(VAlign::V_CENTER, HAlign::H_LEFT);
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

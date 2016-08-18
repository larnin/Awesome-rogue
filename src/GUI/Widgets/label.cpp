#include "label.h"
#include "graphichelper.h"

Label::Label(const AdaptableBounds & _bounds)
    : Widget(_bounds)
    , m_text(_bounds.vAlign, _bounds.hAlign)
{

}

void Label::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::FloatRect rect(bounds.boundsOnView(target.getView()));

    if(m_state == ControlState::ACTIVE)
        target.draw(drawSelector(rect));

    m_text.draw(target, rect);
}

void Label::setText(const std::string & text)
{
    m_text.setText(text);
}

std::string Label::getText() const
{
    return m_text.getText();
}

void Label::setText(const std::string &text, const Font & font, unsigned int size, const sf::Color & color)
{
    m_text.setFont(font);
    m_text.setText(text);
    m_text.setTextSize(size);
    m_text.setTextColor(color);
}

void Label::setFont(const Font & font)
{
    m_text.setFont(font);
}

void Label::setTextSize(unsigned int size)
{
    m_text.setTextSize(size);
}

void Label::setTextColor(const sf::Color & color)
{
    m_text.setTextColor(color);
}

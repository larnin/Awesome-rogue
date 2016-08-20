#include "textdrawer.h"
#include <SFML/Graphics/Sprite.hpp>

TextDrawer::TextDrawer(VAlign _vAlign, HAlign _hAlign)
    : vAlign(_vAlign)
    , hAlign(_hAlign)
    , m_texture(std::make_unique<sf::RenderTexture>())
    , m_toRedraw(true)
{

}

void TextDrawer::draw(sf::RenderTarget &target, sf::FloatRect b) const
{
    //sf::FloatRect b(bounds.boundsOnView(target.getView()));
    redrawText(sf::Vector2f(b.width, b.height));

    sf::Sprite s(m_texture->getTexture());
    s.setPosition(b.left, b.top);
    sf::Vector2f sSize(s.getGlobalBounds().width, s.getGlobalBounds().height);
    if(hAlign == HAlign::H_CENTER)
        s.move(int((b.width-sSize.x)/2.0f), 0);
    if(hAlign == HAlign::H_RIGHT)
        s.move(int(b.width-sSize.x), 0);
    if(vAlign == VAlign::V_CENTER)
        s.move(0, int((b.height-sSize.y)/2.0f));
    if(vAlign == VAlign::V_BOTTOM)
        s.move(0, int(b.height-sSize.y));
    target.draw(s);
}

void TextDrawer::setText(const std::string & text)
{
    m_text.setString(sf::String::fromUtf8(text.begin(), text.end()));
    m_toRedraw = true;
}

std::string TextDrawer::getText() const
{
    return m_text.getString().toAnsiString();
}

void TextDrawer::setText(const std::string & text, const Font & font, unsigned int size, const sf::Color & color, const sf::Color & outlineColor, float outlineThickness)
{
    m_text.setString(sf::String::fromUtf8(text.begin(), text.end()));
    m_text.setFont(*font);
    m_font = font;
    m_text.setCharacterSize(size);
    m_text.setFillColor(color);
    m_text.setOutlineColor(outlineColor);
    m_text.setOutlineThickness(outlineThickness);
    m_toRedraw = true;
}

void TextDrawer::setFont(const Font & font)
{
    m_text.setFont(*font);
    m_font = font;
    m_toRedraw = true;
}

void TextDrawer::setTextSize(unsigned int size)
{
    m_text.setCharacterSize(size);
    m_toRedraw = true;
}

void TextDrawer::setTextColor(const sf::Color & color)
{
    m_text.setFillColor(color);
    m_toRedraw = true;
}

void TextDrawer::setTextOutlineColor(const sf::Color & color)
{
    m_text.setOutlineColor(color);
    m_toRedraw = true;
}

void TextDrawer::setTextOutlineThickness(float value)
{
    m_text.setOutlineThickness(value);
    m_toRedraw = true;
}

void TextDrawer::redrawText(const sf::Vector2f & size) const
{
    if(size.x < 1 || size.y < 1)
        return;

    if(int(size.x) > int(m_texture->getSize().x) || int(size.y) > int(m_texture->getSize().y))
    {
        m_texture->create(std::max((unsigned int)(size.x), m_texture->getSize().x), std::max((unsigned int)(size.y), m_texture->getSize().y));
        m_toRedraw = true;
    }

    if(m_toRedraw)
    {
        const sf::FloatRect textBounds(m_text.getGlobalBounds());
        sf::Vector2f pos;
        if(hAlign == HAlign::H_CENTER)
            pos.x += int((m_texture->getSize().x-textBounds.width)/2.0f);
        if(hAlign == HAlign::H_RIGHT)
            pos.x += int(m_texture->getSize().x-textBounds.width);
        if(vAlign == VAlign::V_CENTER)
            pos.y += int((m_texture->getSize().y-textBounds.height)/2.0f);
        if(vAlign == VAlign::V_BOTTOM)
            pos.y += int(m_texture->getSize().y-textBounds.height);
        m_text.setPosition(pos);

        m_texture->clear(sf::Color(0, 0, 0, 0));
        m_texture->draw(m_text);
        m_texture->display();

        m_toRedraw = false;
    }
}

sf::Text TextDrawer::getCurrentText() const
{
    return m_text;
}

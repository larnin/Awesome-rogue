#ifndef TEXTDRAWER_H
#define TEXTDRAWER_H

#include "GUI/Widgets/widget.h"
#include "Utilities/ressource.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <memory>

class TextDrawer
{
public:
    TextDrawer(VAlign _vAlign, HAlign _hAlign);
    TextDrawer(TextDrawer &&) = default;
    TextDrawer & operator= (TextDrawer &&) = default;

    void draw(sf::RenderTarget &target, sf::FloatRect b) const;

    void setText(const std::string & text);
    std::string getText() const;
    void setText(const std::string &text, const Font & font, unsigned int size, const sf::Color & color, const sf::Color & outlineColor = sf::Color::Transparent, float outlineThickness = 0);
    void setFont(const Font & font);
    void setTextSize(unsigned int size);
    void setTextColor(const sf::Color & color);
    void setTextOutlineColor(const sf::Color & color);
    void setTextOutlineThickness(float value);
    sf::Text getCurrentText() const;

    VAlign vAlign;
    HAlign hAlign;

private:
    void redrawText(const sf::Vector2f & size) const;
    Font m_font;
    mutable std::unique_ptr<sf::RenderTexture> m_texture;
    mutable bool m_toRedraw;
    mutable sf::Text m_text;

};

#endif // TEXTDRAWER_H

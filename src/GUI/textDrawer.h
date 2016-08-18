#ifndef TEXTDRAWER_H
#define TEXTDRAWER_H

#include <memory>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include "GUI/Widgets/widget.h"
#include "Utilities/ressource.h"

class TextDrawer
{
public:
    TextDrawer(VAlign _vAlign, HAlign _hAlign);
    TextDrawer(TextDrawer &&) = default;
    TextDrawer & operator= (TextDrawer &&) = default;

    void draw(sf::RenderTarget &target, sf::FloatRect b) const;

    void setText(const std::string & text);
    std::string getText() const;
    void setText(const std::string &text, const Font & font, unsigned int size, const sf::Color & color);
    void setFont(const Font & font);
    void setTextSize(unsigned int size);
    void setTextColor(const sf::Color & color);
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

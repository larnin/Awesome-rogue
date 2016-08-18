#ifndef LABEL_H
#define LABEL_H

#include "GUI/textDrawer.h"
#include "widget.h"

class Label : public Widget
{
public:
    Label(const AdaptableBounds & _bounds = AdaptableBounds());
    virtual ~Label() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

    void setText(const std::string & text);
    std::string getText() const;
    void setText(const std::string &text, const Font & font, unsigned int size, const sf::Color & color);
    void setFont(const Font & font);
    void setTextSize(unsigned int size);
    void setTextColor(const sf::Color & color);

private:
    TextDrawer m_text;
};

#endif // LABEL_H

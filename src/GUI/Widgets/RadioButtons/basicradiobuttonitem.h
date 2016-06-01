#ifndef SIMPLERADIOBUTTONITEM_H
#define SIMPLERADIOBUTTONITEM_H

#include "abstactradiobuttonitem.h"
#include "GUI/textDrawer.h"
#include "Utilities/ressource.h"

class BasicRadioButtonItem : public AbstactRadioButtonItem
{
public:
    BasicRadioButtonItem();
    BasicRadioButtonItem(BasicRadioButtonItem &&) = default;
    BasicRadioButtonItem & operator=(BasicRadioButtonItem &&) = default;
    virtual ~BasicRadioButtonItem() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates, const sf::FloatRect & bounds) const;

private:
    Texture m_texture;
    Font m_font;
    mutable TextDrawer m_text;
};

#endif // SIMPLERADIOBUTTONITEM_H

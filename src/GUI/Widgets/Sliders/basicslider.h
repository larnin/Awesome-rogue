#ifndef BASICSLIDER_H
#define BASICSLIDER_H

#include "Utilities/ressource.h"
#include "abstractslider.h"
#include "GUI/textDrawer.h"

class BasicSlider : public AbstractSlider
{
public:
    BasicSlider(const AdaptableBounds & _bounds = AdaptableBounds());
    virtual ~BasicSlider() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    Texture m_texture;
    Font m_font;
    mutable TextDrawer m_text;
};

#endif // BASICSLIDER_H

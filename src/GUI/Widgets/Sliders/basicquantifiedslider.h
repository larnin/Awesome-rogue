#ifndef BASICQUANTIFIEDSLIDER_H
#define BASICQUANTIFIEDSLIDER_H

#include "abstractquantifiedslider.h"
#include "Utilities/ressource.h"
#include "GUI/textDrawer.h"

class BasicQuantifiedSlider : public AbstractQuantifiedSlider
{
public:
    BasicQuantifiedSlider(const AdaptableBounds & _bounds = AdaptableBounds());
    virtual ~BasicQuantifiedSlider() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;

private:
    void updateLabels() const;
    Texture m_texture;
    Font m_font;
    mutable std::vector<TextDrawer> m_texts;
};

#endif // BASICQUANTIFIEDSLIDER_H

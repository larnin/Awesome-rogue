#ifndef BASICCOMBOBOX_H
#define BASICCOMBOBOX_H

#include "abstractcombobox.h"
#include "Utilities/ressource.h"
#include "GUI/textDrawer.h"

class BasicCombobox : public AbstractComboBox
{
public:
    BasicCombobox(const AdaptableBounds & _bounds = AdaptableBounds());
    ~BasicCombobox() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    void updateTexts() const;

    Texture m_texture;
    Font m_font;
    mutable TextDrawer m_label;
    mutable std::vector<TextDrawer> m_texts;
};

#endif // BASICCOMBOBOX_H

#ifndef BASICBUTTON_H
#define BASICBUTTON_H

#include <SFML/Graphics/RenderTexture.hpp>
#include "abstractbutton.h"
#include "Utilities/ressource.h"
#include "GUI/textDrawer.h"

class BasicButton : public AbstractButton
{
public:
    BasicButton(const AdaptableBounds & _bounds = AdaptableBounds(), const std::string & text = std::string());
    virtual ~BasicButton() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    Texture m_texture;
    Font m_font;
    TextDrawer m_text;
};

#endif // BASICBUTTON_H

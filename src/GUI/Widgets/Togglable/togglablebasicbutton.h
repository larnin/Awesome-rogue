#ifndef TOGGLABLEBASICBUTTON_H
#define TOGGLABLEBASICBUTTON_H

#include "abstracttogglable.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include "Utilities/ressource.h"
#include "GUI/textDrawer.h"

class TogglableBasicButton : public AbstractTogglable
{
public:
    TogglableBasicButton(const AdaptableBounds & _bounds = AdaptableBounds(), const std::string & text = std::string());
    virtual ~TogglableBasicButton() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    Texture m_texture;
    Font m_font;
    sf::Texture m_textTexture;
    TextDrawer m_text;
};

#endif // TOGGLABLEBASICBUTTON_H

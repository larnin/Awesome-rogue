#ifndef BASICCHECKBOX_H
#define BASICCHECKBOX_H

#include "abstracttogglable.h"
#include "Utilities/ressource.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include "GUI/textDrawer.h"

class BasicCheckbox : public AbstractTogglable
{
public:
    BasicCheckbox(const AdaptableBounds & _bounds = AdaptableBounds(), const std::string & label = std::string());

    ~BasicCheckbox() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    Texture m_texture;
    Font m_font;
    TextDrawer m_text;
};

#endif // BASICCHECKBOX_H

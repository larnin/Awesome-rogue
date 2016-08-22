#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include "Utilities/ressource.h"
#include "Events/eventreceiver.h"
#include "textDrawer.h"

class EventExperienceChanged;

class GameInterface : public sf::Drawable, public EventReceiver
{
public:
    GameInterface();
    virtual ~GameInterface() = default;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    void onExperienceChanged(EventExperienceChanged e);

    Texture m_texture;
    TextDrawer m_text;
};

#endif // GAMEINTERFACE_H

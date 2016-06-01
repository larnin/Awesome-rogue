#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include "Utilities/ressource.h"

class GameInterface : public sf::Drawable
{
public:
    GameInterface();
    virtual ~GameInterface() = default;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    Texture m_texture;
};

#endif // GAMEINTERFACE_H

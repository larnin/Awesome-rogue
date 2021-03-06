#ifndef INTERACTOR_H
#define INTERACTOR_H

#include "Systemes/controlable.h"
#include "Utilities/noncopiable.h"
#include "Map/blockinteractiontype.h"
#include "textDrawer.h"
#include "Utilities/ressource.h"
#include <SFML/Graphics/Drawable.hpp>

class Location;
class Entity;

class Interactor : public Controlable, public sf::Drawable, private NonCopiable
{
public:
    Interactor(std::weak_ptr<Entity> holder);
    Interactor(Interactor &&) = default;
    Interactor & operator =(Interactor &&) = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;
    virtual void control(CommandsValue & v) override;

private:
    std::pair<BlockInteractionType, Location> getInteractable() const;

    std::weak_ptr<Entity> m_holder;
    mutable TextDrawer m_text;
    Font m_font;
};

#endif // INTERACTOR_H

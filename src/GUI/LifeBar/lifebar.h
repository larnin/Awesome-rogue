#ifndef LIFEBAR_H
#define LIFEBAR_H

#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include "Utilities/ressource.h"
#include "Utilities/noncopiable.h"

class Entity;

class LifeBar : public sf::Drawable, private NonCopiable
{
public:
    LifeBar(std::weak_ptr<Entity> e);
    LifeBar(LifeBar &&) = default;
    LifeBar & operator=(LifeBar &&) = default;
    virtual ~LifeBar() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    std::weak_ptr<Entity> m_entity;
    Texture m_texture;
};

#endif // LIFEBAR_H

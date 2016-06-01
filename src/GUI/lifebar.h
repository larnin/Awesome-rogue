#ifndef LIFEBAR_H
#define LIFEBAR_H

#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include "Entities/Types/entity.h"
#include "Utilities/ressource.h"

class LifeBar : public sf::Drawable
{
public:
    LifeBar(std::weak_ptr<Entity> e);
    virtual ~LifeBar() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    std::weak_ptr<Entity> m_entity;
    Texture m_texture;
};

#endif // LIFEBAR_H

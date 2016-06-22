#ifndef SQUAREMOB_H
#define SQUAREMOB_H

#include "entity.h"
#include "Utilities/ressource.h"
#include "Collisions/path.h"

class SquareMob : public Entity
{
public:
    SquareMob(const Location & pos);
    SquareMob(SquareMob&&) = default;
    SquareMob & operator= (SquareMob &&) = default;
    virtual ~SquareMob() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

protected:
    virtual void updateComportement(const sf::Time & elapsedTime);

private:
    Texture m_texture;
    Path m_path;
    bool m_rotationSide;
    float m_projectileTime;
};

#endif // SQUAREMOB_H

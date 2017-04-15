#ifndef SQUAREMOB_H
#define SQUAREMOB_H

#include "entity.h"
#include "Utilities/ressource.h"
#include "Collisions/path.h"

class SquareMob : public Entity
{
public:
    SquareMob(const Location & pos);
    SquareMob(const json & j);
    SquareMob(SquareMob&&) = default;
    SquareMob & operator= (SquareMob &&) = default;
    virtual ~SquareMob() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;

protected:
    virtual void updateComportement(const sf::Time & elapsedTime) override;
    virtual void onKill() override;
    virtual json serialize() const override;

private:
    Texture m_texture;
    Path m_path;
    bool m_rotationSide;
    float m_projectileTime;
};

#endif // SQUAREMOB_H

#ifndef CROSSLASERMOB_H
#define CROSSLASERMOB_H

#include "entity.h"
#include "Utilities/ressource.h"

class CrossLaserMob : public Entity
{
public:
    CrossLaserMob(const Location & pos);
    CrossLaserMob(CrossLaserMob&&) = default;
    CrossLaserMob & operator= (CrossLaserMob &&) = default;
    virtual ~CrossLaserMob() = default;

    virtual void update(const sf::Time & elapsedTime);
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    bool m_rotationSide;
    Texture m_texture;
};

#endif // CROSSLASERMOB_H

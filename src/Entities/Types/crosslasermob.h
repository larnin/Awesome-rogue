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

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

protected:
    virtual void updateComportement(const sf::Time & elapsedTime);
    virtual void onKill();

private:
    bool m_rotationSide;
    Texture m_texture;
    float m_fireTimer;
};

#endif // CROSSLASERMOB_H

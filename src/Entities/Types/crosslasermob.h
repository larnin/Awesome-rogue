#ifndef CROSSLASERMOB_H
#define CROSSLASERMOB_H

#include "entity.h"
#include "Utilities/ressource.h"

class CrossLaserMob : public Entity
{
public:
    CrossLaserMob(const Location & pos);
    CrossLaserMob(const json & j);
    CrossLaserMob(CrossLaserMob&&) = default;
    CrossLaserMob & operator= (CrossLaserMob &&) = default;
    virtual ~CrossLaserMob() = default;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;

protected:
    virtual void updateComportement(const sf::Time & elapsedTime) override;
    virtual void onKill() override;
    virtual json serialize() const override;

private:
    bool m_rotationSide;
    Texture m_texture;
    float m_fireTimer;
};

#endif // CROSSLASERMOB_H

#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "Systemes/controlable.h"
#include "Utilities/ressource.h"
#include "Lights/Types/pointlight.h"

class Player : public Entity, public Controlable
{
public:
    Player(const Location & pos);
    Player(const json & j);
    Player(Player&&) = default;
    Player & operator= (Player &&) = default;
    virtual ~Player() = default;

    virtual void control(CommandsValue & v) override;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;

protected:
    virtual void updateComportement(const sf::Time & elapsedTime) override;
    virtual void onKill() override;

private:
    Texture m_texture;
    sf::Vector2f m_controleDirection;
};

#endif // PLAYER_H

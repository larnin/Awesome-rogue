#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "Systemes/controlable.h"
#include "Utilities/ressource.h"
#include "Collisions/hitbox.h"

class Player : public Entity, public Controlable
{
public:
    Player(const Location & pos);
    Player(Player&&) = default;
    Player & operator= (Player &&) = default;
    virtual ~Player() = default;

    virtual void control(CommandsValue & v);
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

protected:
    virtual void updateComportement(const sf::Time & elapsedTime);

private:
    Texture m_texture;
    sf::Vector2f m_controleDirection;
};

#endif // PLAYER_H

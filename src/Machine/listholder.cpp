#include "listholder.h"

void ListHolder::update(const sf::Time &)
{
    projectiles.clean();
    entities.clean();
    particules.clean();
}

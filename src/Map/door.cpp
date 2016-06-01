#include "door.h"
#include "room.h"

Door::Door(const Location & _pos, const Location & _dest)
    : pos(_pos)
    , dest(_dest)
{

}

bool Door::isValid() const
{
    return pos.isValide() && dest.isValide();
}

Orientation Door::getOrientation() const
{
    auto rLock(pos.getRoom().lock());
    if(!rLock)
        return Orientation::UNORIENTED;

    return orientedSide(pos.getBlockPos(), sf::Rect<unsigned int>(sf::Vector2u(0, 0), rLock->getSize()));
}

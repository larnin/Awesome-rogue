#include "rapidfire.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Utilities/quadrender.h"
#include "Utilities/vect2convert.h"
#include "Map/blocktype.h"
#include "Collisions/collisions.h"

RapidFire::RapidFire(const Location & pos, Team team, const sf::Vector2f & speed)
    : Projectile(pos, team)
    , m_texture("res/img/rapidfire.png")
    , m_speed(speed)
{

}

void RapidFire::update(const sf::Time & elapsedTime)
{
    if(m_killed)
        return;

    sf::Vector2f dir(m_speed*elapsedTime.asSeconds());
    std::shared_ptr<Room> r(m_pos.getRoom().lock());
    if(r)
    {
        auto col(Collisions::interact(getBox(), m_pos.getPos(), dir, r));
        if(col.collision)
            m_killed = true;
    }
    m_pos.move(dir);
}

void RapidFire::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    if(m_killed)
        return;

    float orientation(angle(m_speed));
    sf::Vector2f pos(m_pos.toGlobalPos()*float(BlockType::tileSize));
    sf::VertexArray array(sf::Quads, 4);
    drawOrientedQuad(&array[0], sf::FloatRect(pos.x-8, pos.y-2, 16, 4), sf::FloatRect(0, 0, 16, 4), false, false, Rotation::ROT_0
            , orientation, pos);

    target.draw(array, sf::RenderStates(m_texture()));
}

HitBox RapidFire::getBox() const
{
    HitBox box;
    box.addLine(Line(sf::Vector2f(-0.5f, 0), sf::Vector2f(0.5f, 0)));
    return box.transform(angle(m_speed), false, false);
}

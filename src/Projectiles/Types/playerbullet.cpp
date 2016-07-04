#include "playerbullet.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Utilities/quadrender.h"
#include "Utilities/vect2convert.h"
#include "Map/blocktype.h"
#include "Collisions/collisions.h"
#include "Events/eventgetter.h"

PlayerBullet::PlayerBullet(const Location & pos, Team team, const sf::Vector2f & speed, std::weak_ptr<Entity> sender)
    : Projectile(pos, team, sender)
    , m_texture("res/img/pbullet.png")
    , m_speed(speed)
{

}

void PlayerBullet::update(const sf::Time & elapsedTime)
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

    std::vector<std::shared_ptr<Entity>> entities(EventGetter<std::vector<std::shared_ptr<Entity>>,unsigned int>::get(r->getID()));
    for(std::shared_ptr<Entity> & e : entities)
    {
        if(!e)
            continue;
        if(e->getTeam() == m_team)
            continue;
        if(Collisions::interact(getBox(), m_pos.getPos(), dir, e->getBox(), e->getPos().getPos()).collision)
            if(e->damage(2, m_sender, normalise(m_speed)*2.0f))
            {
                m_killed = true;
                break;
            }
    }
}

void PlayerBullet::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    if(m_killed)
        return;

    float orientation(angle(m_speed));
    sf::Vector2f pos(m_pos.toGlobalPos()*float(BlockType::tileSize));
    sf::VertexArray array(sf::Quads, 4);
    drawOrientedQuad(&array[0], sf::FloatRect(pos.x-8, pos.y-5, 16, 10), sf::FloatRect(0, 0, 16, 10), false, false, Rotation::ROT_0
            , orientation, pos);

    target.draw(array, sf::RenderStates(m_texture()));
}

HitBox PlayerBullet::getBox() const
{
    HitBox box;
    box.addLine(Line(sf::Vector2f(-0.5f, 0.3f), sf::Vector2f(0.5f, 0)));
    box.addLine(Line(sf::Vector2f(-0.5f, -0.3f), sf::Vector2f(0.5f, 0)));
    return box.transform(angle(m_speed), false, false);
}

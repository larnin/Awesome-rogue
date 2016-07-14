#include "bigcircle.h"
#include "Collisions/collisions.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Utilities/quadrender.h>
#include "Map/blocktype.h"
#include "Utilities/vect2convert.h"
#include "Events/eventgetter.h"

const float decreaseTime(1.5f);

BigCircle::BigCircle(const Location & pos, Team team, const sf::Vector2f & speed, float maxTime, std::weak_ptr<Entity> sender)
    : Projectile(pos, team, sender)
    , m_texture("res/img/bigball.png")
    , m_speed(speed)
    , m_lastTime(maxTime)
{
    m_destroyable = true;
}

void BigCircle::update(const sf::Time & elapsedTime)
{
    if(m_killed)
        return;

    m_lastTime -= elapsedTime.asSeconds();
    if(m_lastTime <= 0)
        m_killed = true;

    sf::Vector2f dir(m_speed*elapsedTime.asSeconds());
    std::shared_ptr<Room> r(m_pos.getRoom().lock());
    if(!r)
        return;

    /*auto col(Collisions::interact(getBox(), m_pos.getPos(), dir, r));
    if(col.collision)
        m_killed  = true;*/

    sf::Vector2f pos(m_pos.getPos());
    if(pos.x < -0.5f || pos.x > r->getSize().x - 0.5f || pos.y < 0.5f || pos.y > r->getSize().y - 0.5f)
        m_killed = true;

    m_pos.move(dir);

    std::vector<std::shared_ptr<Entity>> entities(EventGetter<std::vector<std::shared_ptr<Entity>>,unsigned int>::get(r->getID()));
    for(std::shared_ptr<Entity> & e : entities)
    {
        if(!e)
            continue;
        if(e->getTeam() == m_team)
            continue;
        if(Collisions::interact(getBox(), m_pos.getPos(), dir, e->getBox(), e->getPos().getPos()).collision)
            if(e->damage(50, m_sender, normalise(m_speed)*10.0f))
            {
                m_killed = true;
                break;
            }
    }
}

void BigCircle::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    if(m_killed)
        return;
    sf::Vector2f pos(m_pos.toGlobalPos()*float(BlockType::tileSize));
    float radius(16.0f);

    sf::VertexArray array(sf::Quads, 4);
    drawQuad(&array[0], sf::FloatRect(pos.x-radius, pos.y-radius, 2*radius, 2*radius), sf::FloatRect(0, 0, 2*radius, 2*radius));
    target.draw(array, sf::RenderStates(m_texture()));
}

HitBox BigCircle::getBox() const
{
    HitBox box;
    const float radius(1.0f);
    for(int i(0) ; i < 8 ; i++)
    {
        float a(3.14159/4*(i+0.5f));
        float nA(3.14159/4*(i+1.5f));
        box.addLine(Line(sf::Vector2f(radius*std::cos(a), radius*std::sin(a)), sf::Vector2f(radius*std::cos(nA), radius*std::sin(nA))));
    }
    return box;
}

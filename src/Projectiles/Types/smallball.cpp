#include "smallball.h"
#include "Collisions/collisions.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Utilities/quadrender.h>
#include "Map/blocktype.h"
#include "Utilities/vect2convert.h"

SmallBall::SmallBall(const Location & pos, Team team, const sf::Vector2f & speed, unsigned int bounces, float maxTime)
    : Projectile(pos, team)
    , m_texture("res/img/smallball.png")
    , m_speed(speed)
    , m_bounces(bounces)
    , m_lastTime(maxTime)
{
    m_destroyable = true;
}

void SmallBall::update(const sf::Time & elapsedTime)
{
    if(m_killed)
        return;

    m_lastTime -= elapsedTime.asSeconds();
    if(m_lastTime <= 0)
        m_killed = true;

    sf::Vector2f dir(m_speed*elapsedTime.asSeconds());
    std::shared_ptr<Room> r(m_pos.getRoom().lock());
    if(r)
    {
        auto col(Collisions::interact(getBox(), m_pos.getPos(), dir, r));
        if(col.collision)
        {
            if(m_bounces == 0)
                m_killed = true;
            else
            {
                dir = col.endPos-m_pos.getPos();
                float a(col.surfaceAngle - angle(m_speed));

                m_speed = toVect(norm(m_speed), angle(m_speed) + 2*a);
                m_bounces--;
            }
        }
    }
    m_pos.move(dir);
}

void SmallBall::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    if(m_killed)
        return;
    sf::Vector2f pos(m_pos.toGlobalPos()*float(BlockType::tileSize));
    float radius(3.0f);

    sf::VertexArray array(sf::Quads, 4);
    std::uniform_int_distribution<unsigned int> distrib(0, 2);
    unsigned int offset(distrib(m_randEngine)*7);
    drawQuad(&array[0], sf::FloatRect(pos.x-radius, pos.y-radius, 2*radius, 2*radius), sf::FloatRect(offset, 0, 6, 6));
    target.draw(array, sf::RenderStates(m_texture()));
}

HitBox SmallBall::getBox() const
{
    HitBox box;
    const float radius(0.18);
    for(int i(0) ; i < 8 ; i++)
    {
        float a(3.14159/4*(i+0.5f));
        float nA(3.14159/4*(i+1.5f));
        box.addLine(Line(sf::Vector2f(radius*std::cos(a), radius*std::sin(a)), sf::Vector2f(radius*std::cos(nA), radius*std::sin(nA))));
    }
    return box;
}

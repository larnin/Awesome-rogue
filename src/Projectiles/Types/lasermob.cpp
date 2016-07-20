#include "lasermob.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Utilities/vect2convert.h"
#include "Utilities/quadrender.h"
#include "Map/room.h"
#include "Map/blocktype.h"
#include "Collisions/collisions.h"
#include "Events/eventgetter.h"
#include "Entities/Types/entity.h"

const float totalTime(2.5f);
const float preTime(1.0f);
const float fireTime(totalTime-preTime);

LaserMob::LaserMob(const Location & pos, Team team, std::weak_ptr<Entity> sender, float orientationOffset)
    : Projectile(pos, team, sender)
    , m_texture("res/img/laserMob.png")
    , m_orientationOffset(orientationOffset)
    , m_orientation(orientationOffset)
    , m_totalTime(0)
    , m_size(0)
{
    std::shared_ptr<Entity> e(m_sender.lock());
    if(e)
        m_orientation = e->getOrientation() + m_orientationOffset;
}

void LaserMob::update(const sf::Time & elapsedTime)
{
    std::shared_ptr<Entity> e(m_sender.lock());
    if(!e)
    {
        m_killed = true;
        return;
    }
     m_totalTime += elapsedTime.asSeconds();
     if(m_totalTime > totalTime)
         m_killed = true;

     m_pos = e->getPos();
     m_orientation = e->getOrientation() + m_orientationOffset;


     std::shared_ptr<Room> r(m_pos.getRoom().lock());
     if(!r)
         return;
     float size(maxDistInRect(r->getSize()));
     auto result(Collisions::interact(m_pos.getPos(), toVect(size, m_orientation), r));
     if(result.collision)
         size = norm(result.endPos-m_pos.getPos());
     m_size = size;

     if(m_totalTime > preTime)
     {
        std::vector<std::shared_ptr<Entity>> entities(EventGetter<std::vector<std::shared_ptr<Entity>>,unsigned int>::get(r->getID()));
        sf::Vector2f laserVect(toVect(size, m_orientation));
        for(std::shared_ptr<Entity> & e : entities)
        {
            if(!e)
                continue;
            if(e->getTeam() == m_team)
                continue;

            if(Collisions::interact(m_pos.getPos(), laserVect, e->getBox(), e->getPos().getPos()).collision)
                e->damage(elapsedTime.asSeconds()*100.0f, m_sender);
        }
     }
}

void LaserMob::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    if(m_killed)
        return;

    std::uniform_int_distribution<unsigned int> distrib(0, 2);
    const float offset(distrib(m_randEngine)*10);
    sf::VertexArray array(sf::Quads, 8);
    const float width(m_totalTime < preTime ? 0.5f : 2.5f);
    sf::Vector2f startPos(m_pos.toGlobalPos()*float(BlockType::tileSize));
    drawOrientedQuad(&array[0], sf::FloatRect(startPos.x, startPos.y-width, -m_size*BlockType::tileSize, 2*width)
            , sf::FloatRect(offset, 0, 1, 5), false, false, Rotation::ROT_0, m_orientation, startPos);

    sf::Vector2f endPos(startPos+toVect(m_size, m_orientation)*float(BlockType::tileSize));
    const float endRadius(4);
    const float multiplier(m_totalTime < preTime ? 0.3f : 1.0f);
    drawQuad(&array[4], sf::FloatRect(endPos.x-endRadius*multiplier, endPos.y-endRadius*multiplier, endRadius*2*multiplier, endRadius*2*multiplier)
            , sf::FloatRect(offset+2, 0, endRadius*2, endRadius*2));

    target.draw(array, sf::RenderStates(m_texture()));
}

float LaserMob::maxDistInRect(const sf::Vector2u & size) const
{
    float lenght(size.x+size.y);
    HitBox box;
    box.addLine(Line(sf::Vector2f(-0.5, -0.5f), sf::Vector2f(size.x-0.5f, -0.5f)));
    box.addLine(Line(sf::Vector2f(size.x-0.5f, -0.5f), sf::Vector2f(size.x-0.5f, size.y-0.5f)));
    box.addLine(Line(sf::Vector2f(size.x-0.5f, size.y-0.5f), sf::Vector2f(-0.5f, size.y-0.5f)));
    box.addLine(Line(sf::Vector2f(-0.5, size.y-0.5f), sf::Vector2f(-0.5f, -0.5f)));
    auto result(Collisions::interact(m_pos.getPos(), toVect(lenght, m_orientation), box, sf::Vector2f(0, 0)));
    if(!result.collision)
        return lenght;
    return norm(result.endPos-m_pos.getPos());
}

#include "delayedpunch.h"
#include <SFML/Graphics/VertexArray.hpp>
#include "Utilities/quadrender.h"
#include "Map/blocktype.h"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Events/eventgetter.h"
#include "Utilities/vect2convert.h"
#include "Map/room.h"

const float totalTime(2.0f);
const float frameTime(0.05f);
const float waitTime(totalTime-5*frameTime);

DelayedPunch::DelayedPunch(const Location & pos, Team team, std::weak_ptr<Entity> sender)
    : Projectile(pos, team, sender)
    , m_texture("res/img/punchballAttack.png")
{
    if(!m_sender.lock())
        m_killed = true;

    m_destroyable = false;
}

void DelayedPunch::update(const sf::Time & elapsedTime)
{
    m_lifeTime += elapsedTime;
    if(m_lifeTime.asSeconds()>=totalTime)
    {
        m_killed = true;
        return;
    }

    std::shared_ptr<Entity> e(m_sender.lock());
    if(!e)
    {
        m_killed = true;
        return;
    }
    m_pos = e->getPos();

    std::shared_ptr<Room> r(m_pos.getRoom().lock());
    if(!r)
        return;

    std::vector<std::shared_ptr<Entity>> entities(EventGetter<std::vector<std::shared_ptr<Entity>>,unsigned int>::get(r->getID()));
    for(std::shared_ptr<Entity> & entity : entities)
    {
        if(!entity)
            continue;
        if(entity->getTeam() == m_team)
            continue;
        sf::Vector2f vect(entity->getPos().getPos() - m_pos.getPos());
        if(norm(vect) < 1.5)
            entity->damage(10, m_sender, normalise(vect));
    }
}

void DelayedPunch::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    if(m_killed)
        return;

    const float life(m_lifeTime.asSeconds());
    sf::VertexArray array(sf::Quads, 4);
    sf::Vector2f pos(m_pos.toGlobalPos()*float(BlockType::tileSize));
    if(life < waitTime)
    {
        float size(life/waitTime*BlockType::tileSize*2.0f);
        std::uniform_real_distribution<float> distrib(0.9f, 1.1f);
        size *= distrib(m_randEngine);

        drawQuad(&array[0], sf::FloatRect(pos.x-size, pos.y-size, size*2, size*2), sf::FloatRect(138, 0, 47, 47));
    }
    else
    {
        const float size(45.0f/1.5f);
        int frame((life-waitTime)/frameTime);
        bool fliped(false);
        float position(1);
        switch(frame)
        {
        case 0:
        default:
        break;
        case 1:
            position = 47;
        break;
        case 2:
            position = 93;
        break;
        case 3:
            position = 47;
            fliped = true;
        break;
        case 4:
            fliped = true;
        break;
        }
        drawQuad(&array[0], sf::FloatRect(pos.x-size, pos.y-size, size*2, size*2), sf::FloatRect(position, 0, 45, 45), fliped, fliped);
    }
    target.draw(array, sf::RenderStates(m_texture()));
}

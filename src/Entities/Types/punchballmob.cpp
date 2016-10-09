#include "punchballmob.h"
#include "Utilities/vect2convert.h"
#include "Map/blocktype.h"
#include "Utilities/quadrender.h"
#include "Entities/entitylist.h"
#include "Events/eventgetter.h"
#include "Projectiles/projectilefactory.h"
#include "Projectiles/Types/delayedpunch.h"
#include "Entities/Types/player.h"
#include "Particules/particulefactory.h"
#include "Particules/Types/mobdeath.h"
#include "Events/Datas/eventplayerchangeroom.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

PunchBallMob::PunchBallMob(const Location & pos)
    : Entity(pos)
    , m_texture("res/img/mobs.png")
    , m_fireTime(0)
    , m_onFire(false)
{
    for(int i(0) ; i < 8 ; i++)
    {
        float a(3.14159/4*(i+0.5f));
        float nA(3.14159/4*(i+1.5f));
        m_originalBox.addLine(Line(sf::Vector2f(0.4f*std::cos(a), 0.4f*std::sin(a)), sf::Vector2f(0.4f*std::cos(nA), 0.4f*std::sin(nA))));
    }

    m_currentBox = m_originalBox;

    m_maxLife = 10.0f;
    m_life = m_maxLife;
    m_team = Team::MOB_TEAM;
    m_activeDistance = 2;
    m_canPassDoor = true;

    auto p(EventSimpleGetter<std::shared_ptr<Player>>::get());
    if(p)
        if(p->getPos().getRoom().lock() == m_pos.getRoom().lock())
            m_target = p;

    m_drops = experienceToItems(8);
}

void PunchBallMob::updateComportement(const sf::Time & elapsedTime)
{
    float maxFireTime(2.0f);
    float fireTimeColdown(1.0f);
    float distanceTrigFire(2.0f);
    const float accelerationNorm(10.0f);
    const float limitMultiplier(1.5f);
    const float limitReductor(1.0f);
    const float epsilon(0.01f);

    std::shared_ptr<Room> r(m_pos.getRoom().lock());
    if(!r)
        return;

    sf::Vector2f targetPos(m_pos.getPos());
    std::shared_ptr<Entity> target(m_target.lock());
    if(target)
    {
        if(m_path.dest().getRoom().lock() != target->getPos().getRoom().lock())
            recreatePath();
        else
        {
            if(m_path.dest().getBlockPos() != target->getPos().getBlockPos())
                recreatePath();
            if(norm(m_pos.getPos()-target->getPos().getPos()) < distanceTrigFire && m_fireTime > maxFireTime + fireTimeColdown)
            {
                m_onFire = true;
                ProjectileFactory::createSend<DelayedPunch>(getPos(), m_team, EventGetter<std::shared_ptr<Entity>, unsigned int>::get(getID()));
                m_fireTime = 0;
            }
        }

    }

    if(!m_path.finished())
        targetPos = m_path.next(m_pos).getPos();

    if(m_onFire)
    {
        targetPos = m_pos.getPos();
        if(m_fireTime >= maxFireTime)
            m_onFire = false;
    }
    m_fireTime += elapsedTime.asSeconds();

    sf::Vector2f acceleration(0.0f, 0.0f);
    if(norm(targetPos-m_pos.getPos()) > epsilon)
        acceleration = toVect(accelerationNorm, angle(targetPos-m_pos.getPos()));

    m_speed += acceleration*elapsedTime.asSeconds();
    float n(norm(m_speed)*(1-(limitMultiplier*elapsedTime.asSeconds()))-(limitReductor*elapsedTime.asSeconds()));
    if(n < 0)
        n = 0;
    m_speed = toVect(n, angle(m_speed));

    execMove(m_speed*elapsedTime.asSeconds());
}

void PunchBallMob::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::VertexArray render(sf::Quads, 4);
    sf::Vector2f globalPos(m_pos.toGlobalPos()*float(BlockType::tileSize));
    sf::FloatRect rect(-0.5f*13+globalPos.x, -0.5f*13+globalPos.y, 13, 13);
    drawOrientedQuad(&render[0], rect, sf::FloatRect(49, 0, 13 ,13), true, false, Rotation::ROT_0, m_orientation, globalPos);
    target.draw(render, sf::RenderStates(m_texture()));
}

void PunchBallMob::onAwake()
{
    connect<EventPlayerChangeRoom>(std::bind(&PunchBallMob::onPlayerChangeRoom, this, _1));
}

void PunchBallMob::onDisable()
{
    disconnect();
}

void PunchBallMob::onPlayerChangeRoom(EventPlayerChangeRoom e)
{
    std::shared_ptr<Entity> target(m_target.lock());
    if(!target)
        m_target = EventGetter<std::shared_ptr<Entity>, unsigned int>::get(e.entityID);
}

void PunchBallMob::recreatePath()
{
    std::shared_ptr<Entity> target(m_target.lock());
    if(!target)
        return;
    m_path.newPath(getPos(), target->getPos());
}

void PunchBallMob::onKill()
{
    ParticuleFactory::createSend<MobDeath>(m_pos, m_texture, sf::FloatRect(49, 0, 13, 13), 0, m_speed);
}

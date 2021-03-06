#include "circlemob.h"
#include "Map/blocktype.h"
#include "Utilities/quadrender.h"
#include "Utilities/vect2convert.h"
#include "Entities/entitylist.h"
#include "Events/eventgetter.h"
#include "Projectiles/projectilefactory.h"
#include "Projectiles/Types/cacaura.h"
#include "Entities/Types/player.h"
#include "Entities/entityfactory.h"
#include "Particules/particulefactory.h"
#include "Particules/Types/mobdeath.h"
#include "Events/Datas/Entity/eventplayerchangeroom.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

CircleMob::CircleMob(const Location & pos, bool small)
    : Entity(pos, SERIALIZE_CIRCLEMOB)
    , m_texture("res/img/mobs.png")
    , m_lastRandDirTime(0)
    , m_randAngle(0)
    , m_isSmall(small)
{
    const float radius(m_isSmall ? 0.28 : 0.45f);
    for(int i(0) ; i < 8 ; i++)
    {
        float a(3.14159/4*(i+0.5f));
        float nA(3.14159/4*(i+1.5f));
        m_originalBox.addLine(Line(sf::Vector2f(radius*std::cos(a), radius*std::sin(a)), sf::Vector2f(radius*std::cos(nA), radius*std::sin(nA))));
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

    m_drops = experienceToItems(5);
}

CircleMob::CircleMob(const json & j)
    : Entity(j, SERIALIZE_CIRCLEMOB)
    , m_texture("res/img/mobs.png")
    , m_lastRandDirTime(0)
    , m_randAngle(0)
    , m_isSmall(j["small"])
{

}

void CircleMob::updateComportement(const sf::Time & elapsedTime)
{
    const float accelerationNorm(m_isSmall ? 8.0f : 6.0f);
    const float limitMultiplier(m_isSmall ? 1.2f : 1.0f);
    const float limitReductor(m_isSmall ? 0.9f : 0.8f);
    const float epsilon(0.01f);
    const float maxRandDirTime(1.0f);

    std::shared_ptr<Room> r(m_pos.getRoom().lock());
    if(!r)
        return;

    sf::Vector2f targetPos(m_pos.getPos());
    std::shared_ptr<Entity> target(m_target.lock());
    if(target)
    {
        if(m_path.dest().getRoom().lock() != target->getPos().getRoom().lock())
            recreatePath();
        else if(m_path.dest().getBlockPos() != target->getPos().getBlockPos())
                recreatePath();

    }

    if(!m_path.finished())
        targetPos = m_path.next(m_pos).getPos();

    m_lastRandDirTime += elapsedTime.asSeconds();
    if(m_lastRandDirTime > maxRandDirTime)
    {
        m_lastRandDirTime = 0;
        std::uniform_real_distribution<float> distrib(-1.0f, 1.0f);
        m_randAngle = distrib(m_randEngine);
    }

    sf::Vector2f acceleration(0.0f, 0.0f);
    if(norm(targetPos-m_pos.getPos()) > epsilon)
        acceleration = toVect(accelerationNorm, angle(targetPos-m_pos.getPos())+m_randAngle);

    m_speed += acceleration*elapsedTime.asSeconds();
    float n(norm(m_speed)*(1-(limitMultiplier*elapsedTime.asSeconds()))-(limitReductor*elapsedTime.asSeconds()));
    if(n < 0)
        n = 0;
    m_speed = toVect(n, angle(m_speed));

    execMove(m_speed*elapsedTime.asSeconds());

    if(target)
    {
        std::shared_ptr<Projectile> p(m_projectile.lock());
        if(!p)
            m_projectile = ProjectileFactory::createSend<CacAura>(getPos(), m_team, EventGetter<std::shared_ptr<Entity>, unsigned int>::get(getID())
                                                       , m_isSmall ? 0.5f : 0.7f);
    }
}

void CircleMob::onKill()
{
    if(!m_isSmall)
    {
        unsigned int nb(std::uniform_int_distribution<unsigned int>(2, 4)(m_randEngine));
        std::uniform_real_distribution<float> dRandDir(-5.0f, 5.0f);
        for(unsigned int i(0) ; i < nb ; i++)
        {
            auto eList(EntityFactory::create(EntityType::E_SMALL_CIRCLE_MOB, m_pos));
            for(auto & e : eList)
                e->push(sf::Vector2f(dRandDir(m_randEngine), dRandDir(m_randEngine)));
        }
        ParticuleFactory::createSend<MobDeath>(m_pos, m_texture, sf::FloatRect(63, 0, 15, 15), 0, m_speed);
    }
    else ParticuleFactory::createSend<MobDeath>(m_pos, m_texture, sf::FloatRect(79, 0, 9, 9), 0, m_speed);
}

void CircleMob::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::VertexArray render(sf::Quads, 4);
    sf::Vector2f globalPos(m_pos.toGlobalPos()*float(BlockType::tileSize));
    const float size(m_isSmall ? 9 : 15);
    const float offset(m_isSmall ? 79 : 63);

    sf::FloatRect rect(-0.5f*size+globalPos.x, -0.5f*size+globalPos.y, size, size);
    drawOrientedQuad(&render[0], rect, sf::FloatRect(offset, 0, size ,size), true, false, Rotation::ROT_0, m_orientation, globalPos);
    target.draw(render, sf::RenderStates(m_texture()));
}

void CircleMob::onAwake()
{
    connect<EventPlayerChangeRoom>(std::bind(&CircleMob::onPlayerChangeRoom, this, _1));
}

void CircleMob::onDisable()
{
    disconnect();
}

void CircleMob::onPlayerChangeRoom(EventPlayerChangeRoom e)
{
    std::shared_ptr<Entity> target(m_target.lock());
    if(!target)
        m_target = EventGetter<std::shared_ptr<Entity>, unsigned int>::get(e.entityID);
}

void CircleMob::recreatePath()
{
    std::shared_ptr<Entity> target(m_target.lock());
    if(!target)
        return;
    m_path.newPath(getPos(), target->getPos());
}

json CircleMob::serialize() const
{
    json j(Entity::serialize());
    j["small"] = m_isSmall;
    return j;
}

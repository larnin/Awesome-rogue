#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "trackermob.h"
#include "Map/blocktype.h"
#include "Utilities/quadrender.h"
#include "Utilities/vect2convert.h"
#include "Collisions/pathfinder.h"
#include "Collisions/collisions.h"
#include "Entities/entitylist.h"
#include "Events/eventgetter.h"
#include "Projectiles/projectilefactory.h"
#include "Projectiles/Types/rapidfire.h"
#include "Entities/Types/player.h"
#include "Particules/particulefactory.h"
#include "Particules/Types/mobdeath.h"
#include "Events/Datas/eventplayerchangeroom.h"
#include "Map/room.h"

TrackerMob::TrackerMob(const Location & pos)
    : Entity(pos)
    , m_texture("res/img/mobs.png")
    , m_canfire(false)
    , m_projectilesToFire(3)
    , m_timeToFire(1)
{
    m_originalBox.addLine(Line(sf::Vector2f(-0.45f, -0.375f), sf::Vector2f(-0.375f, -0.45f)));
    m_originalBox.addLine(Line(sf::Vector2f(-0.375f, -0.45f), sf::Vector2f(0.375f, -0.45f)));
    m_originalBox.addLine(Line(sf::Vector2f(0.375f, -0.45f), sf::Vector2f(0.45f, -0.375f)));
    m_originalBox.addLine(Line(sf::Vector2f(0.45f, -0.375f), sf::Vector2f(0.45f, 0.375f)));
    m_originalBox.addLine(Line(sf::Vector2f(0.45f, 0.375f), sf::Vector2f(0.375f, 0.45f)));
    m_originalBox.addLine(Line(sf::Vector2f(0.375f, 0.45f), sf::Vector2f(-0.375f, 0.45f)));
    m_originalBox.addLine(Line(sf::Vector2f(-0.375f, 0.45f), sf::Vector2f(-0.45f, 0.375f)));
    m_originalBox.addLine(Line(sf::Vector2f(-0.45f, 0.375f), sf::Vector2f(-0.45f, -0.375f)));

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
}

void TrackerMob::updateComportement(const sf::Time & elapsedTime)
{
    std::shared_ptr<Room> r(m_pos.getRoom().lock());
    if(!r)
        return;

    std::shared_ptr<Entity> target(m_target.lock());
    if(target)
    {
        if(target->getPos().getRoom().lock() != m_targetPos.getRoom().lock()
                || !m_targetPos.isValide()
                || target->getPos().getBlockPos() != m_targetPos.getBlockPos())
            recreatePath();
    }

    sf::Vector2f targetPos(m_pos.getPos());
    if(m_path.finished())
        m_canfire = true;
    else targetPos = m_path.next(m_pos).getPos();

    const float accelerationNorm(6.0f);
    const float limitMultiplier(1.0f);
    const float limitReductor(0.5f);
    //const float rotationSpeed(1.0f - 2.0f*(m_rotationSide));
    const float epsilon(0.01f);

    sf::Vector2f acceleration(0.0f, 0.0f);
    if(norm(targetPos-m_pos.getPos()) > epsilon)
        acceleration = toVect(accelerationNorm, angle(targetPos-m_pos.getPos()));

    m_speed += acceleration*elapsedTime.asSeconds();
    float n(norm(m_speed)*(1-(limitMultiplier*elapsedTime.asSeconds()))-(limitReductor*elapsedTime.asSeconds()));
    if(n < 0)
        n = 0;
    m_speed = toVect(n, angle(m_speed));

    execMove(m_speed*elapsedTime.asSeconds());

    if(m_canfire)
    {
        m_timeToFire-= elapsedTime.asSeconds();
        if(m_timeToFire < 0)
        {
            if(target)
            {
                sf::Vector2f dir(normalise(target->getPos().getPos() - getPos().getPos())*10.0f);
                ProjectileFactory::createSend<RapidFire>(getPos(), m_team, dir, EventGetter<std::shared_ptr<Entity>, unsigned int>::get(getID()));
            }
            if(m_projectilesToFire == 1)
            {
                m_timeToFire = 2.0f;
                m_projectilesToFire = 3;
            }
            else
            {
                m_timeToFire = 0.15f;
                m_projectilesToFire--;
            }
        }
    }
}

void TrackerMob::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::VertexArray render(sf::Quads, 4);
    sf::Vector2f globalPos(m_pos.toGlobalPos()*float(BlockType::tileSize));
    sf::FloatRect rect(-0.5f*(BlockType::tileSize-1)+globalPos.x, -0.5f*(BlockType::tileSize-1)+globalPos.y, (BlockType::tileSize-1), (BlockType::tileSize-1));
    drawOrientedQuad(&render[0], rect, sf::FloatRect(17, 0, 15 ,15), true, false, Rotation::ROT_0, m_orientation, globalPos);
    target.draw(render, sf::RenderStates(m_texture()));
}

void TrackerMob::onPlayerChangeRoom(EventPlayerChangeRoom e)
{
    std::shared_ptr<Entity> target(m_target.lock());
    if(!target)
        m_target = EventGetter<std::shared_ptr<Entity>,unsigned int>::get(e.entityID);
}

void TrackerMob::onAwake()
{
    connect<EventPlayerChangeRoom>(std::bind(&TrackerMob::onPlayerChangeRoom, this, _1));
}

void TrackerMob::onDisable()
{
    disconnect();
}

void TrackerMob::recreatePath()
{
    m_canfire = false;

    std::shared_ptr<Entity> target(m_target.lock());
    if(!target)
        return;

    m_targetPos = target->getPos();

    sf::Vector2u localPathStart(getPos().getBlockPos());

    if(target->getPos().getRoom().lock() != getPos().getRoom().lock())
    {
        auto path(PathFinder::pathRooms(getPos().getRoom(), target->getPos().getRoom()));
        if(path.size() < 2)
        {
            m_path = Path();
            return;
        }
        const auto r(path.back().lock());
        const auto & doors(path[path.size()-2].lock()->doors());
        auto it(std::find_if(doors.begin(), doors.end(), [r](const auto & door){return r == door.dest.getRoom().lock();}));
        if(it == doors.end())
        {
            m_path = Path();
            return;
        }
        localPathStart = it->dest.getBlockPos();
    }

    std::shared_ptr<Room> r(target->getPos().getRoom().lock());
    auto movePath(PathFinder::path(r, localPathStart, target->getPos().getBlockPos()));
    if(movePath.empty())
    {
        m_path = Path();
        return;
    }

    auto it(std::find_if(movePath.begin(), movePath.end(), [target, r](const auto & pos)
    {
        sf::Vector2f fPos(pos);
        return !Collisions::interact(fPos, target->getPos().getPos()-fPos, r).collision;
    }));
    if(it == movePath.end())
        m_path.newPath(getPos(), target->getPos());
    else m_path.newPath(getPos(), Location(*it, r));
}

void TrackerMob::onKill()
{
    ParticuleFactory::createSend<MobDeath>(m_pos, m_texture, sf::FloatRect(17, 0, 15, 15), 0, m_speed);
}

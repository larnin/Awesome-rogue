#include "crosslasermob.h"
#include "Utilities/vect2convert.h"
#include "Collisions/collisions.h"
#include "Map/blocktype.h"
#include "Utilities/quadrender.h"
#include "Projectiles/projectilefactory.h"
#include "Projectiles/Types/lasermob.h"
#include "Events/eventgetter.h"
#include "Particules/particulefactory.h"
#include "Particules/Types/mobdeath.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

const float speed(1.0f);

CrossLaserMob::CrossLaserMob(const Location & pos)
    : Entity(pos, SERIALIZE_CROSSLASERMOB)
    , m_texture("res/img/mobs.png")
    , m_fireTimer(0)
{
    m_originalBox.addLine(Line(sf::Vector2f(-0.5f, 0.0f), sf::Vector2f(0.0f, -0.5f)));
    m_originalBox.addLine(Line(sf::Vector2f(0.0f, -0.5f), sf::Vector2f(0.5f, 0.0f)));
    m_originalBox.addLine(Line(sf::Vector2f(0.5f, 0.0f), sf::Vector2f(0.0f, 0.5f)));
    m_originalBox.addLine(Line(sf::Vector2f(0.0f, 0.5f), sf::Vector2f(-0.5f, 0.0f)));

    m_currentBox = m_originalBox;

    m_maxLife = 10.0f;
    m_life = m_maxLife;
    m_team = Team::MOB_TEAM;
    m_activeDistance = 1;
    m_canPassDoor = false;

    std::uniform_int_distribution<int> d(0, 1);
    m_rotationSide = d(m_randEngine) == 0;

    std::uniform_real_distribution<float> a(0, 2*3.14159);
    m_orientation = a(m_randEngine);

    m_speed = toVect(speed, m_orientation);

    std::uniform_real_distribution<float> distrib(-5, 0);
    m_fireTimer = distrib(m_randEngine);

    m_drops = experienceToItems(10);
}

CrossLaserMob::CrossLaserMob(const json & j)
    : Entity(j, SERIALIZE_CROSSLASERMOB)
    , m_rotationSide(j["rotside"])
    , m_texture("res/img/mobs.png")
    , m_fireTimer(0)
{

}

void CrossLaserMob::updateComportement(const sf::Time & elapsedTime)
{
    std::shared_ptr<Room> r(m_pos.getRoom().lock());
    if(!r)
        return;

    sf::Vector2f localSpeed(m_speed*elapsedTime.asSeconds());

    Collisions::InteractResult result(Collisions::interact(m_currentBox, m_pos.getPos(), localSpeed, m_pos.getRoom().lock()));
    if(!result.collision)
        m_pos.move(localSpeed);
    else
    {
        sf::Vector2f localMove(result.endPos-m_pos.getPos());
        m_pos.move(localMove);
        float a(result.surfaceAngle - angle(m_speed));

        m_speed = toVect(norm(m_speed), angle(m_speed) + 2*a);
        m_rotationSide = !m_rotationSide;
    }

    const float rotationSpeed(0.5f - 1.0f*(m_rotationSide));
    float newAngle(m_orientation + rotationSpeed*elapsedTime.asSeconds());
    execRotate(newAngle);

    m_fireTimer += elapsedTime.asSeconds();
    if(m_fireTimer > 6.0f)
    {
        m_fireTimer = 0;
        for(int i(0) ; i < 4 ; i++)
        {
            float a(3.14159/2*(i));
            ProjectileFactory::createSend<LaserMob>(getPos(), m_team, EventGetter<std::shared_ptr<Entity>, unsigned int>::get(getID()), a);
        }
    }
}

void CrossLaserMob::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::VertexArray render(sf::Quads, 4);
    sf::Vector2f globalPos(m_pos.toGlobalPos()*float(BlockType::tileSize));
    sf::FloatRect rect(-0.5f*(BlockType::tileSize-1)+globalPos.x, -0.5f*(BlockType::tileSize-1)+globalPos.y, (BlockType::tileSize-1), (BlockType::tileSize-1));
    drawOrientedQuad(&render[0], rect, sf::FloatRect(33, 0, 15 ,15), true, false, Rotation::ROT_0, m_orientation, globalPos);
    target.draw(render, sf::RenderStates(m_texture()));
}

void CrossLaserMob::onKill()
{
    ParticuleFactory::createSend<MobDeath>(m_pos, m_texture, sf::FloatRect(33, 0, 15, 15), m_orientation, m_speed);
}

json CrossLaserMob::serialize() const
{
    json j(Entity::serialize());
    j["rotside"] = m_rotationSide;
    return j;
}

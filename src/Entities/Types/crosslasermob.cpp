#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "crosslasermob.h"
#include "Utilities/vect2convert.h"
#include "Collisions/collisions.h"
#include "Map/blocktype.h"
#include "Utilities/quadrender.h"

const float speed(1.0f);

CrossLaserMob::CrossLaserMob(const Location & pos)
    : Entity(pos)
    , m_texture("res/img/mobs.png")
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
}

void CrossLaserMob::update(const sf::Time & elapsedTime)
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

    const float rotationSpeed(1.0f - 2.0f*(m_rotationSide));
    float newAngle(m_orientation + rotationSpeed*elapsedTime.asSeconds());
    execRotate(newAngle);
}

void CrossLaserMob::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::VertexArray render(sf::Quads, 4);
    sf::Vector2f globalPos(m_pos.toGlobalPos()*float(BlockType::tileSize));
    sf::FloatRect rect(-0.5f*(BlockType::tileSize-1)+globalPos.x, -0.5f*(BlockType::tileSize-1)+globalPos.y, (BlockType::tileSize-1), (BlockType::tileSize-1));
    drawOrientedQuad(&render[0], rect, sf::FloatRect(33, 0, 15 ,15), true, false, Rotation::ROT_0, m_orientation, globalPos);
    target.draw(render, sf::RenderStates(m_texture()));
}

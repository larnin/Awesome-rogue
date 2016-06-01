#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "squaremob.h"
#include "Utilities/vect2convert.h"
#include "Collisions/collisions.h"
#include "Map/blocktype.h"
#include "Utilities/quadrender.h"

SquareMob::SquareMob(const Location & pos)
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
}

void SquareMob::update(const sf::Time & elapsedTime)
{
    std::shared_ptr<Room> r(m_pos.getRoom().lock());
    if(!r)
        return;

    sf::Vector2f targetPos(m_pos.getPos());

    if(m_path.finished())
    {
        std::uniform_int_distribution<int> distrib(-2, 2);
        sf::Vector2i pathEnd(sf::Vector2i(m_pos.getBlockPos())+sf::Vector2i(distrib(m_randEngine), distrib(m_randEngine)));
        if(pathEnd.x >= 0 && pathEnd.y >= 0 && pathEnd.x < int(r->getSize().x) && pathEnd.y < int(r->getSize().y))
            if(getBoxType((*r)(sf::Vector2u(pathEnd)).boxCaracts) == BoxType::EMPTY)
                m_path.newPath(m_pos, Location(pathEnd, r));
    }
    else targetPos = m_path.next(m_pos).getPos();

    const float accelerationNorm(0.2f);
    const float limitMultiplier(0.2f);
    const float limitReductor(0.1f);
    const float rotationSpeed(1.0f - 2.0f*(m_rotationSide));
    const float epsilon(0.01f);

    sf::Vector2f acceleration(0.0f, 0.0f);
    if(norm(targetPos-m_pos.getPos()) > epsilon)
        acceleration = toVect(accelerationNorm, angle(targetPos-m_pos.getPos()));

    m_speed += acceleration*elapsedTime.asSeconds();
    float n(norm(m_speed)*(1-(limitMultiplier*elapsedTime.asSeconds()))-(limitReductor*elapsedTime.asSeconds()));
    if(n < 0)
        n = 0;
    m_speed = toVect(n, angle(m_speed));

    execMove();

    float newAngle(m_orientation + rotationSpeed*elapsedTime.asSeconds());
    execRotate(newAngle);
}

void SquareMob::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::VertexArray render(sf::Quads, 4);
    sf::Vector2f globalPos(m_pos.toGlobalPos()*float(BlockType::tileSize));
    sf::FloatRect rect(-0.5f*BlockType::tileSize+globalPos.x, -0.5f*BlockType::tileSize+globalPos.y, BlockType::tileSize, BlockType::tileSize);
    drawOrientedQuad(&render[0], rect, sf::FloatRect(0, 0, 16 ,16), true, false, Rotation::ROT_0, m_orientation, globalPos);
    target.draw(render, sf::RenderStates(m_texture()));
}

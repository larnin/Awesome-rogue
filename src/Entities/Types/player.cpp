#include "player.h"
#include "Utilities/vect2convert.h"
#include "Collisions/collisions.h"
#include "Utilities/quadrender.h"
#include "Map/blocktype.h"
#include "Events/Datas/eventcenterofviewchanged.h"
#include "Events/Datas/eventplayerchangeroom.h"
#include "Events/Datas/eventpreplayerchangeroom.h"
#include "Events/event.h"
#include "Particules/particulefactory.h"
#include "Particules/Types/mobdeath.h"
#include "Controles/commandsvalue.h"
#include "events/Datas/eventaddlight.h"
#include "Lights/Types/circlecoloredlight.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

const float PI(3.14159f);

Player::Player(const Location & pos)
    : Entity(pos)
    , Controlable(ControlState::ACTIVE)
    , m_texture("res/img/player.png")
    , m_controleDirection(0, 0)
{
    m_originalBox.addLine(Line(sf::Vector2f(-0.5f, 0), sf::Vector2f(-0.19f, -0.31f)));
    m_originalBox.addLine(Line(sf::Vector2f(-0.19f, -0.31f), sf::Vector2f(0.5f, 0)));
    m_originalBox.addLine(Line(sf::Vector2f(0.5f, 0), sf::Vector2f(-0.19f, 0.31f)));
    m_originalBox.addLine(Line(sf::Vector2f(-0.19f, 0.31f), sf::Vector2f(-0.5f, 0)));

    m_currentBox = m_originalBox;

    m_damageable = true;
    m_maxLife = 100.0f;
    m_life = m_maxLife;
    m_maxShield = 50.0f;
    m_shield = m_maxShield;
    m_team = Team::PLAYER_TEAM;
    m_activeDistance = 1.0f;
    m_canPassDoor = true;
    m_invincibleTime = 0.2f;

    m_light = std::make_shared<CircleColoredLight>(m_pos.toGlobalPos()*float(BlockType::tileSize)
                                                   , sf::Color::White, 250, sf::Color(50, 0, 0), 50);
    Event<EventAddLight>::send(EventAddLight(m_light));
}

void Player::control(CommandsValue & v)
{
    m_controleDirection = sf::Vector2f(v.getValue(CommandType::MOVE_RIGHT) - v.getValue(CommandType::MOVE_LEFT)
                                       , v.getValue(CommandType::MOVE_DOWN) - v.getValue(CommandType::MOVE_UP));
    float n(norm(m_controleDirection));
    if(n > 1.0f)
        m_controleDirection /= n;
}

void Player::updateComportement(const sf::Time & elapsedTime)
{
    const float controlMultiplier(12.0f);
    const float limitMultiplier(1.5f);
    const float limitReductor(1.0f);
    const float mincontroleRot(0.1f);

    m_speed += m_controleDirection*controlMultiplier*elapsedTime.asSeconds();
    float n(norm(m_speed)*(1-(limitMultiplier*elapsedTime.asSeconds()))-(limitReductor*elapsedTime.asSeconds()));
    if(n < 0)
        n = 0;
    m_speed = toVect(n, angle(m_speed));

    std::shared_ptr<Room> rOld(getPos().getRoom().lock());
    execMove(m_speed*elapsedTime.asSeconds());
    std::shared_ptr<Room> rNew(getPos().getRoom().lock());
    if(rOld != rNew)
    {
        Event<EventPrePlayerChangeRoom>::send(EventPrePlayerChangeRoom(getID()));
        Event<EventPlayerChangeRoom>::send(EventPlayerChangeRoom(getID()));
    }

    Event<EventCenterOfViewChanged>::send(EventCenterOfViewChanged(m_pos.toGlobalPos()*float(BlockType::tileSize)));

    float newAngle(m_orientation);
    if(std::abs(m_speed.x) > mincontroleRot || std::abs(m_speed.y) > mincontroleRot)
        newAngle = angle(m_speed);
    if(std::abs(m_controleDirection.x) > mincontroleRot || std::abs(m_controleDirection.y) > mincontroleRot)
        newAngle = angle(m_controleDirection);
    execRotate(newAngle);

    m_light->setPos(m_pos.toGlobalPos()*float(BlockType::tileSize));
}

void Player::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::VertexArray render(sf::Quads, 4);
    sf::Vector2f globalPos(m_pos.toGlobalPos()*float(BlockType::tileSize));
    sf::FloatRect rect(-0.5f*BlockType::tileSize+globalPos.x, -0.31f*BlockType::tileSize+globalPos.y, BlockType::tileSize, 0.62f*BlockType::tileSize);
    drawOrientedQuad(&render[0], rect, sf::FloatRect(0, 0, 16 ,10), true, false, Rotation::ROT_0, m_orientation, globalPos);
    target.draw(render, sf::RenderStates(m_texture()));
}

void Player::onKill()
{
    ParticuleFactory::createSend<MobDeath>(m_pos, m_texture, sf::FloatRect(0, 0, 16, 10), m_orientation+3.14159f, m_speed);
}

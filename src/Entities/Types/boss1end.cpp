#include "boss1end.h"
#include "Events/eventgetter.h"
#include "Events/event.h"
#include "Events/Datas/eventplaycameraeffect.h"
#include "Events/Datas/eventsetbosslifebar.h"
#include "Events/Datas/eventbosskilled.h"
#include "player.h"
#include "Particules/particulefactory.h"
#include "Particules/Types/boss1transition.h"
#include "Particules/Types/particule.h"
#include "Map/blocktype.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "Utilities/quadrender.h"
#include "Projectiles/projectilefactory.h"
#include "Projectiles/Types/bigcircle.h"
#include "Particules/particulefactory.h"
#include "Particules/Types/mobdeath.h"
#include "Utilities/vect2convert.h"
#include "Map/room.h"

const float PI(3.14159f);
const float minFireDelay(1.0f);
const float maxFireDelay(2.0f);
const unsigned int nbProjectiles(8);
const float projectileSpeed(1.5f);

Boss1End::Boss1End(const Location & pos)
    : Entity(pos)
    , m_texture("res/img/boss1.png")
{
     auto p(EventSimpleGetter<std::shared_ptr<Player>>::get());
     if(p)
         m_target = p;

     m_maxLife = 200.0f;
     m_lifeRegeneration = 5.0f;
     m_life = m_maxLife;
     m_team = Team::MOB_TEAM;
     m_activeDistance = 0;
     m_canPassDoor = false;
     m_showLifeOnDamage = false;

     m_knockbackMultiplier = 0.0f;

     const float radius(1.7f);
     for(int i(0) ; i < 12 ; i++)
     {
         float a(3.14159/8*(i+0.5f));
         float nA(3.14159/8*(i+1.5f));
         m_originalBox.addLine(Line(sf::Vector2f(radius*std::cos(a), radius*std::sin(a)), sf::Vector2f(radius*std::cos(nA), radius*std::sin(nA))));
     }
     m_currentBox = m_originalBox;

     m_time = std::uniform_real_distribution<float>(minFireDelay, maxFireDelay)(m_randEngine);

     m_drops = experienceToItems(500);
}

void Boss1End::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::Vector2f pos(m_pos.toGlobalPos() * float(BlockType::tileSize));
    float size(54);
    sf::VertexArray array(sf::Quads, 4);
    drawQuad(&array[0], sf::FloatRect(pos.x-size/2, pos.y-size/2, size, size), sf::FloatRect(280, 34, size, size));

    target.draw(array, sf::RenderStates(m_texture()));
}

void Boss1End::updateComportement(const sf::Time & elapsedTime)
{
    m_time -= elapsedTime.asSeconds();
    if(m_time <= 0)
    {
        fire();
        m_time = std::uniform_real_distribution<float>(minFireDelay, maxFireDelay)(m_randEngine);
    }
}

void Boss1End::onKill()
{
    std::shared_ptr<Room> r(m_pos.getRoom().lock());
    if(r)
        r->openDoors();
    Event<EventPlayCameraEffect>::send(EventPlayCameraEffect(CameraEffectType::EFFECT_HARD_SHAKE, 0.1f));
    ParticuleFactory::createSend<MobDeath>(m_pos, m_texture, sf::FloatRect(280, 34, 54, 54), 0, sf::Vector2f(0, 0));
    Event<EventSetBossLifeBar>::send(EventSetBossLifeBar(std::shared_ptr<BossLifeBar>()));
    Event<EventBossKilled>::send(EventBossKilled());
}

void Boss1End::fire()
{
    auto sender(EventGetter<std::shared_ptr<Entity>, unsigned int>::get(getID()));

    if(std::uniform_int_distribution<unsigned int>(0, 2)(m_randEngine) == 0)
    {
        float angle(std::uniform_real_distribution<float>(0, 2*PI)(m_randEngine));
        for(unsigned int i(0) ; i < nbProjectiles ; i++)
            ProjectileFactory::createSend<BigCircle>(m_pos, m_team, toVect(projectileSpeed, angle+(2*i*PI/nbProjectiles)), 100, sender);
    }
    else
    {
        std::shared_ptr<Entity> target(m_target.lock());
        if(!target)
            return;
        sf::Vector2f dir(normalise(target->getPos().getPos() - m_pos.getPos())*projectileSpeed*1.5f);
        ProjectileFactory::createSend<BigCircle>(m_pos, m_team, dir, 100, sender);
    }
}


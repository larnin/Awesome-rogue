#include "boss1partslifebar.h"
#include "Entities/Types/entity.h"
#include "Utilities/vect2convert.h"
#include "Utilities/quadrender.h"
#include "Particules/particulefactory.h"
#include "Particules/Types/boss1transition.h"
#include "Events/event.h"
#include "Events/Datas/eventinteraction.h"
#include "Events/Datas/eventremoveentity.h"
#include "cassert"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>

Boss1PartsLifeBar::Boss1PartsLifeBar(const std::vector<std::weak_ptr<Entity>> & entities)
    : BossLifeBar(entities)
    , m_texture("res/img/lifebar_boss1.png")
    , m_onTransition(false)
{

}

void Boss1PartsLifeBar::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    float life(0);
    float maxLife(0);

    for(const auto & e : m_entities)
    {
        std::shared_ptr<Entity> entity(e.lock());
        if(!entity)
            continue;
        if(entity)
            life += entity->getLife();
        maxLife += entity->getMaxLife();
    }

    const sf::View &v(target.getView());
    sf::Vector2f vSize(v.getSize());
    sf::Vector2f vTopLeft(v.getCenter()-vSize/2.0f);

    const sf::Vector2f size(108, 14);
    const sf::Vector2f colorSize(104, 11);
    sf::VertexArray array(sf::Quads, 8);
    drawQuad(&array[0], sf::FloatRect(vTopLeft.x+vSize.x/2-size.x/2, vTopLeft.y+5, size.x, size.y), sf::FloatRect(0, 0, size.x, size.y));

    const float width(colorSize.x*(life/maxLife));
    drawQuad(&array[4], sf::FloatRect(vTopLeft.x+vSize.x/2-width/2, vTopLeft.y+6, width, colorSize.y)
                      , sf::FloatRect(2+(colorSize.x-width)/2, 128, width, colorSize.y));
    array[4].position.x += colorSize.y;
    array[4].texCoords.x += colorSize.y;
    array[5].position.x -= colorSize.y;
    array[5].texCoords.x -= colorSize.y;
    if(array[4].position.x > array[5].position.x)
    {
        array[4].position.x += array[5].position.x;
        array[4].position.x /= 2;
        array[5].position.x = array[4].position.x;
        array[4].texCoords.x += array[5].texCoords.x;
        array[4].texCoords.x /= 2;
        array[5].texCoords.x = array[4].texCoords.x;
    }

    target.draw(array, sf::RenderStates(m_texture()));

}

void Boss1PartsLifeBar::update(const sf::Time &)
{
    bool onLife(false);
    for(auto & e : m_entities)
    {
        auto eLock(e.lock());
        if(!eLock)
            continue;
        if(eLock->damageable())
            onLife = true;
    }
    if(!onLife && !m_onTransition)
        onEndPart();

    if(m_onTransition)
    {
        if(!m_transition.lock())
            Event<EventInteraction>::send(EventInteraction(m_pos, BlockInteractionType::BI_START_BOSS1_FINAL));
    }
}

void Boss1PartsLifeBar::onEndPart()
{
    assert(!m_entities.empty());
    std::shared_ptr<Entity> e(m_entities[0].lock());
    assert(e);

    m_pos = e->getPos();
    m_onTransition = true;

    for(const auto & e : m_entities)
    {
        std::shared_ptr<Entity> entity(e.lock());
        if(entity)
            Event<EventRemoveEntity>::send(EventRemoveEntity(entity));
    }

    m_transition = ParticuleFactory::createSend<Boss1Transition>(e->getPos(), e->getOrientation(), 7, 5);
}

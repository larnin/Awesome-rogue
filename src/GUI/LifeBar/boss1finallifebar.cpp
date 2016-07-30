#include "boss1finallifebar.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Utilities/quadrender.h"
#include "Utilities/vect2convert.h"
#include "Entities/Types/entity.h"

const float frameTime(0.1f);

Boss1FinalLifeBar::Boss1FinalLifeBar(const std::vector<std::weak_ptr<Entity> > & entities)
    : BossLifeBar(entities)
    , m_texture("res/img/lifebar_boss1.png")
    , m_time(0)
    , m_animation(true)
{
    m_animation.addFrame(Frame(frameTime, sf::FloatRect(0, 0, 108, 14), sf::Vector2f(54, 0)));
    m_animation.addFrame(Frame(frameTime, sf::FloatRect(0, 14, 108, 16), sf::Vector2f(54, 0)));
    m_animation.addFrame(Frame(frameTime, sf::FloatRect(0, 30, 108, 18), sf::Vector2f(54, 0)));
    m_animation.addFrame(Frame(frameTime, sf::FloatRect(0, 48, 108, 20), sf::Vector2f(54, 0)));
    m_animation.addFrame(Frame(frameTime, sf::FloatRect(0, 68, 108, 20), sf::Vector2f(54, 0)));
    m_animation.addFrame(Frame(frameTime, sf::FloatRect(0, 88, 108, 20), sf::Vector2f(54, 0)));
}

void Boss1FinalLifeBar::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::VertexArray array(sf::Quads);

    const sf::View &v(target.getView());
    sf::Vector2f vSize(v.getSize());
    sf::Vector2f vTopLeft(v.getCenter()-vSize/2.0f);

    if(m_time <= m_animation.getAnimationTime())
    {
        array.resize(4);
        Frame f(m_animation.getAt(m_time));
        drawQuad(&array[0], sf::FloatRect(vTopLeft.x + vSize.x/2 - f.offset.x, vTopLeft.y+5, f.texRect.width, f.texRect.height), f.texRect);
    }
    else
    {
        array.resize(12);
        sf::FloatRect back(0, 108, 108, 20);
        sf::FloatRect up(10, 139, 88, 2.5);
        sf::FloatRect down(10, 141.5, 88, 3.5);
        drawQuad(&array[0], sf::FloatRect(vTopLeft.x + vSize.x/2 - back.width/2, vTopLeft.y+5, back.width, back.height), back);

        float life(0);
        float maxLife(0);
        for(const auto & entity : m_entities)
        {
            std::shared_ptr<Entity> e(entity.lock());
            if(!e)
                continue;
            life += e->getLife();
            maxLife += e->getMaxLife();
        }

        float l(life/maxLife);

        drawQuad(&array[4], sf::FloatRect(vTopLeft.x+vSize.x/2-up.width/2*l, vTopLeft.y+15, up.width*l, up.height)
                , sf::FloatRect(up.left+(1-l)*up.width/2, up.top, up.width*l, up.height));
        drawQuad(&array[8], sf::FloatRect(vTopLeft.x+vSize.x/2-down.width/2*l, vTopLeft.y+15+up.height, down.width*l, down.height)
                , sf::FloatRect(down.left+(1-l)*down.width/2, down.top, down.width*l, down.height));

        array[4].position.x += up.height;
        array[4].texCoords.x += up.height;
        array[5].position.x -= up.height;
        array[5].texCoords.x -= up.height;

        if(array[4].position.x > array[5].position.x)
        {
            array[4].position.x += array[5].position.x;
            array[4].position.x /= 2;
            array[5].position.x = array[4].position.x;

            array[4].texCoords.x += array[5].texCoords.x;
            array[4].texCoords.x /= 2;
            array[5].texCoords.x = array[4].texCoords.x;
        }

        array[11].position.x += down.height;
        array[11].texCoords.x += down.height;
        array[10].position.x -= down.height;
        array[10].texCoords.x -= down.height;

        if(array[11].position.x > array[10].position.x)
        {
            array[11].position.x += array[10].position.x;
            array[11].position.x /= 2;
            array[10].position.x = array[11].position.x;

            array[11].texCoords.x += array[10].texCoords.x;
            array[11].texCoords.x /= 2;
            array[10].texCoords.x = array[11].texCoords.x;
        }
    }

    target.draw(array, sf::RenderStates(m_texture()));
}

void Boss1FinalLifeBar::update(const sf::Time &elapsedTime)
{
    m_time += elapsedTime.asSeconds();
}

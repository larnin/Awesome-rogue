#include "boss1transition.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "Utilities/quadrender.h"
#include "Utilities/vect2convert.h"
#include "Map/blocktype.h"

const float frameTime(0.2f);
const float PI(3.14159f);
const float minDist(20.0f);
const unsigned int nbBalls(8);

Boss1Transition::Boss1Transition(const Location & pos, float originalOrientation, float distance, float time)
    : Particule(pos)
    , m_texture("res/img/boss1.png")
    , m_time(time)
    , m_startTime(time)
    , m_animation(true)
    , m_startOrientation(fmod(originalOrientation, 2*PI/nbBalls))
    , m_startDistance(distance)
{
    m_animation.addFrame(Frame(frameTime, sf::FloatRect(0, 34, 52, 52), sf::Vector2f(26, 26)));
    m_animation.addFrame(Frame(frameTime, sf::FloatRect(52, 34, 48, 48), sf::Vector2f(24, 24)));
    m_animation.addFrame(Frame(frameTime, sf::FloatRect(100, 34, 44, 44), sf::Vector2f(22, 22)));
    m_animation.addFrame(Frame(frameTime, sf::FloatRect(144, 34, 40, 40), sf::Vector2f(20, 20)));
    m_animation.addFrame(Frame(frameTime, sf::FloatRect(184, 34, 46, 46), sf::Vector2f(23, 23)));
    m_animation.addFrame(Frame(frameTime, sf::FloatRect(230, 34, 50, 50), sf::Vector2f(25, 25)));
    m_animation.addFrame(Frame(frameTime, sf::FloatRect(280, 34, 54, 54), sf::Vector2f(27, 27)));
}

void Boss1Transition::update(const sf::Time & elapsedTime)
{
    m_time -= elapsedTime.asSeconds();
    if(m_time <= 0)
        m_killed = true;
}

void Boss1Transition::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::VertexArray array(sf::Quads);
    sf::Vector2f pos(m_pos.toGlobalPos() * float(BlockType::tileSize));

    if(m_time <= m_animation.getAnimationTime())
    {
        array.resize(4);
        Frame f(m_animation.getAt(m_animation.getAnimationTime()- m_time));
        drawQuad(&array[0], sf::FloatRect(pos.x-f.offset.x, pos.y-f.offset.y, f.texRect.width, f.texRect.height), f.texRect);
    }
    else
    {
        float currentLeftTime((m_time-m_animation.getAnimationTime())/(m_startTime-m_animation.getAnimationTime()));
        float optimizedTime(currentLeftTime > 0.5f
                            ? 2*(1-((currentLeftTime-1)*(currentLeftTime-1)))-1
                            : 2*(currentLeftTime*currentLeftTime));
        float distance((m_startDistance*BlockType::tileSize-minDist)*optimizedTime+minDist);
        float orientation(m_startOrientation*optimizedTime);

        array.resize(nbBalls*4);
        for(unsigned int i(0) ; i < nbBalls ; i++)
        {
            const float size(14);
            float angle(2*PI*i/nbBalls+orientation);
            sf::Vector2f current(toVect(distance, angle)+pos);
            drawQuad(&array[4*i], sf::FloatRect(current.x-size/2, current.y-size/2, size, size), sf::FloatRect(104, 0, size, size));
        }
    }

    target.draw(array, sf::RenderStates(m_texture()));
}

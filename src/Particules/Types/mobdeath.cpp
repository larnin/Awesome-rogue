#include "mobdeath.h"
#include "Utilities/quadrender.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "Map/blocktype.h"
#include "Utilities/vect2convert.h"

const float minTime(0.5f);
const float maxTime(1.5f);

MobDeath::MobDeath(const Location & pos, const Texture & texture, const sf::FloatRect & rect, float angle, const sf::Vector2f & speed)
    : Particule(pos)
    , m_texture(texture)
    , m_lifeTime(0)
{
    generateParts(rect, angle, speed);
}

void MobDeath::generateParts(const sf::FloatRect & rect, float angle, const sf::Vector2f & speed)
{
    m_parts.clear();

    const float partSize(4.0f);
    std::uniform_real_distribution<float> randSpeed(-5.0f, 5.0f);
    std::uniform_real_distribution<float> randTime(minTime, maxTime);
    std::uniform_real_distribution<float> randOrientation(-2.0f, 2.0f);

    sf::Vector2f centerRect(rect.width/2.0f, rect.height/2.0f);
    for(float i(0) ; i < rect.width ; i+= partSize)
        for(float j(0) ; j < rect.height ; j+= partSize)
        {
            sf::Vector2f size(std::min(partSize, rect.width-i), std::min(partSize, rect.height-i));
            sf::Vector2f pos(i, j);
            pos -= centerRect;
            pos += sf::Vector2f(size.x-partSize, size.y-partSize);
            pos = rotate(pos, angle);

            m_parts.push_back(MobDeath::Part(angle, pos, sf::FloatRect(rect.left+i, rect.top+j, size.x, size.y)
                                             , sf::Vector2f(randSpeed(m_randEngine), randSpeed(m_randEngine))+speed
                                             , randOrientation(m_randEngine), randTime(m_randEngine)));
        }

}

void MobDeath::update(const sf::Time & elapsedTime)
{
    float time(elapsedTime.asSeconds());
    m_lifeTime += time;
    if(m_lifeTime > maxTime)
        m_killed = true;

    for(MobDeath::Part & p : m_parts)
    {
        p.offset += p.speed*time;
        p.orientation += p.rotationSpeed*time;
        if(m_lifeTime < p.maxLifeTime-minTime)
            p.alpha = 255;
        else if(m_lifeTime > p.maxLifeTime)
            p.alpha = 0;
        else
        {
            float alphaLife((p.maxLifeTime-m_lifeTime)/minTime);
            p.alpha = (1-alphaLife)*255;
        }
    }
}

void MobDeath::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::Vector2f pos(m_pos.toGlobalPos()*float(BlockType::tileSize));

    sf::VertexArray array(sf::Quads, m_parts.size()*4);
    for(unsigned int i(0) ; i < m_parts.size() ; i++)
    {
        const MobDeath::Part & p(m_parts[i]);
        sf::Vector2f rectPos(pos + p.offset - sf::Vector2f(p.texRect.width, p.texRect.height)/2.0f);
        drawOrientedQuad(&array[4*i], sf::FloatRect(rectPos, sf::Vector2f(p.texRect.width, p.texRect.height)), p.texRect
                , true, true, Rotation::ROT_0, p.orientation, rectPos+sf::Vector2f(p.texRect.width,p.texRect.height)/2.0f);
        for(unsigned int j(0) ; j < 4 ; j++)
            array[4*i+j].color.a = p.alpha;
    }
    target.draw(array, sf::RenderStates(m_texture()));
}

/*void MobDeath::generateParts(const sf::FloatRect & rect, float angle, const sf::Vector2f & speed)
{
    const float partSize(4.0f);

    std::uniform_real_distribution<float> randSpeed(-5.0f, 5.0f);
    std::uniform_real_distribution<float> randTime(minTime, maxTime);
    std::uniform_real_distribution<float> randOrientation(-2.0f, 2.0f);

    sf::Vector2f centerRect(rect.width/2.0f, rect.height/2.0f);

    for(float i(0) ; i < rect.width ; i+= partSize)
        for(float j(0) ; j < rect.height ; j+= partSize)
        {
            sf::Vector2f size(std::min(partSize, rect.width-i), std::min(partSize, rect.height-i));
            sf::Vector2f offset(sf::Vector2f(i+partSize/2, j+partSize/2) - centerRect);
            offset = rotate(offset, angle);
            m_parts.push_back(MobDeath::Part(angle, offset, sf::FloatRect(rect.left+i, rect.top+j, size.x, size.y)
                                             , sf::Vector2f(randSpeed(m_randEngine), randSpeed(m_randEngine))+speed
                                             , randOrientation(m_randEngine), randTime(m_randEngine)));
        }
}

void MobDeath::update(const sf::Time & elapsedTime)
{
    float time(elapsedTime.asSeconds());
    m_lifeTime+= time;

    if(m_lifeTime > maxTime)
        m_killed = true;

    for(MobDeath::Part & p : m_parts)
    {
        p.offset += p.speed*time;
        p.orientation += p.rotationSpeed*time;
        if(m_lifeTime < p.maxLifeTime-minTime)
            p.alpha = 255;
        if(m_lifeTime > p.maxLifeTime)
            p.alpha = 0;
        else p.alpha = (m_lifeTime - (p.maxLifeTime-minTime))/minTime*255;
    }
}

void MobDeath::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::Vector2f pos(m_pos.toGlobalPos()*float(BlockType::tileSize));

    sf::VertexArray array(sf::Quads, m_parts.size()*4);
    for(unsigned int i(0) ; i < m_parts.size() ; i++)
    {
        const MobDeath::Part & p(m_parts[i]);
        sf::Vector2f rectPos(pos + p.offset - sf::Vector2f(p.texRect.width, p.texRect.height)/2.0f);
        drawOrientedQuad(&array[4*i], sf::FloatRect(rectPos, sf::Vector2f(p.texRect.width, p.texRect.height)), p.texRect
                , true, true, Rotation::ROT_0, p.orientation, rectPos+sf::Vector2f(p.texRect.width,p.texRect.height)/2.0f);
        for(unsigned int j(0) ; j < 4 ; j++)
            array[4*i+j].color.a = p.alpha;
    }
    target.draw(array, sf::RenderStates(m_texture()));
}
*/

#include "mobspawn.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "Utilities/quadrender.h"
#include "Map/blocktype.h"
#include <algorithm>

const float initTime(0.3f);
const float lightTime(0.3f);
const float incantSpawnDelta(0.02f);
const float incantLife(0.6f);

MobSpawn::MobSpawn(const Location & pos, float timeToSpawn)
    : Particule(pos)
    , m_totalTime(timeToSpawn)
    , m_lifeTime(0)
    , m_incantTexture("res/img/spawn_incant.png")
    , m_lightTexture("res/img/spawn_light.png")
{

}

void MobSpawn::update(const sf::Time & elapsedTime)
{
    m_lifeTime += elapsedTime.asSeconds();
    if(m_lifeTime > m_totalTime + lightTime)
        m_killed = true;

    updateIncant(elapsedTime);
}

void MobSpawn::updateIncant(const sf::Time & elapsedTime)
{
    m_timeFromLastIncantSpawn += elapsedTime.asSeconds();
    if(m_timeFromLastIncantSpawn > incantSpawnDelta)
    {
        m_timeFromLastIncantSpawn = 0;
        std::uniform_real_distribution<float> alphaDistrib(0.15f, 0.25f);
        std::uniform_real_distribution<float> offsetXDistrib(-0.7f, 0.7f);
        std::uniform_real_distribution<float> offsetYDisrib(-2.0f, 0.5f);
        m_incantAura.push_back(MobSpawn::IncantPart(alphaDistrib(m_randEngine)
                                                    ,sf::Vector2f(offsetXDistrib(m_randEngine), offsetYDisrib(m_randEngine))));
    }

    float time(elapsedTime.asSeconds());

    for(MobSpawn::IncantPart & part : m_incantAura)
    {
        part.lifeTime += time;
        part.offset = part.offsetSpeed*part.lifeTime;
        part.alpha = part.lifeTime / incantLife * part.maxAlpha;
    }

    auto it(std::remove_if(m_incantAura.begin(), m_incantAura.end(), [](const auto & part){return part.lifeTime >= incantLife;}));
    m_incantAura.erase(it, m_incantAura.end());
}

void MobSpawn::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    if(m_lifeTime < m_totalTime)
    {
        std::uniform_int_distribution<int> distrib(180, 230);
        int alpha(distrib(m_randEngine));
        float size(1.0f);
        if(m_lifeTime < initTime)
            size = m_lifeTime / initTime;
        if(m_lifeTime > m_totalTime - initTime)
            size = (m_totalTime - m_lifeTime) / initTime;
        sf::VertexArray array(sf::Quads, (m_incantAura.size()+1)*4);
        sf::Vector2f pos(m_pos.toGlobalPos()*float(BlockType::tileSize));
        sf::FloatRect rect(pos.x-20*size, pos.y-20*size, 40*size, 40*size);
        drawQuad(&array[0], rect, sf::FloatRect(0, 0, 40, 40));
        for(unsigned int i(0) ; i < 4 ; i++)
            array[i].color.a = alpha;
        for(unsigned int i(0) ; i < m_incantAura.size() ; i++)
        {
            const MobSpawn::IncantPart & p(m_incantAura[i]);
            sf::Vector2f pos((m_pos.toGlobalPos()+p.offset)*float(BlockType::tileSize));
            sf::FloatRect rect(pos.x-20*size, pos.y-20*size, 40*size, 40*size);
            drawQuad(&array[4*(i+1)], rect, sf::FloatRect(0, 0, 40, 40));
            for(unsigned int j(0) ; j < 4 ; j++)
                array[(i+1)*4+j].color.a = p.alpha*255;
        }

        target.draw(array, sf::RenderStates(m_incantTexture()));
    }
    if(m_lifeTime > m_totalTime - lightTime)
    {
        float size(1.0f);
        if(m_lifeTime < m_totalTime)
            size = (lightTime - (m_totalTime - m_lifeTime)) / lightTime;
        else size = (lightTime - (m_lifeTime - m_totalTime)) / lightTime;
        float orientation(m_lifeTime*10.0f);
        sf::Vector2f pos(m_pos.toGlobalPos()*float(BlockType::tileSize));
        sf::FloatRect rect(pos.x-50*size, pos.y-50*size, 100*size, 100*size);
        sf::VertexArray array(sf::Quads, 4);
        drawOrientedQuad(&array[0], rect, sf::FloatRect(0, 0, 100, 100), false, false, Rotation::ROT_0, orientation, pos);
        target.draw(array, sf::RenderStates(m_lightTexture()));
    }
}

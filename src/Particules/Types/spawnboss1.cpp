#include "spawnboss1.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "Map/blocktype.h"
#include "Utilities/vect2convert.h"
#include "Utilities/quadrender.h"

const float startLaserTime(0.3f);
const unsigned int beamsCount(8);
const float PI(3.14159f);
const unsigned int framesInLaserTexture(12);
const float frameTime(0.05f);
const float bossSpawnTime(4.0f);
const float maxWavesHeight(64.0f);
const float waveSpeed(5.0f);
const float waveWidth(12.0f);

SpawnBoss1::SpawnBoss1(const Location & pos, float timeToSpawn, float radius)
    : Particule(pos)
    , m_time(0)
    , m_maxTime(timeToSpawn)
    , m_radius(radius)
    , m_bossTexture("res/img/boss1.png")
    , m_laserTexture("res/img/lightning_beam.png")
{
    std::uniform_int_distribution<unsigned int> d(0, framesInLaserTexture-1);
    for(unsigned int i(0) ; i < beamsCount ; i++)
        m_offsetBeam.push_back(d(m_randEngine));
}

void SpawnBoss1::update(const sf::Time & elapsedTime)
{
    m_time += elapsedTime.asSeconds();
    if(m_time >= m_maxTime)
        m_killed = true;
}

void SpawnBoss1::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::VertexArray arrayLaser(sf::Quads, beamsCount*4);

    sf::Vector2f startPos(m_pos.toGlobalPos()*float(BlockType::tileSize));

    float height(m_laserTexture->getSize().y);
    float width(m_laserTexture->getSize().x/12);

    float dist(m_radius*BlockType::tileSize);
    float drawWidth(width/height*dist);
    if(m_time < startLaserTime)
        drawWidth *= (m_time/startLaserTime);
    if(m_maxTime - m_time < startLaserTime)
        drawWidth *= ((m_maxTime - m_time)/startLaserTime);

    sf::VertexArray arrayBalls(sf::Quads);

    for(unsigned int i(0) ; i < beamsCount ; i++)
    {
        unsigned int texIndex(((unsigned int)(m_time/frameTime)+m_offsetBeam[i])%framesInLaserTexture);

        float orientation(2*PI*i/beamsCount);
        drawOrientedQuad(&arrayLaser[4*i], sf::FloatRect(startPos.x, startPos.y-drawWidth/2, dist, drawWidth)
                , sf::FloatRect(texIndex*width, 0, width, height), false, false, Rotation::ROT_90, orientation, startPos);

        drawBossPart(startPos + toVect(dist, orientation), arrayBalls);
    }

    target.draw(arrayLaser, sf::RenderStates(m_laserTexture()));
    target.draw(arrayBalls, sf::RenderStates(m_bossTexture()));
}

void SpawnBoss1::drawBossPart(const sf::Vector2f & pos, sf::VertexArray & array) const
{
    if(m_maxTime-m_time > bossSpawnTime)
        return;

    float normalisedTime((bossSpawnTime-(m_maxTime-m_time))/bossSpawnTime);
    int alpha(normalisedTime*255);
    float waveHeight(maxWavesHeight*(1-normalisedTime));
    float wavePos(m_time*waveSpeed);
    float radius(17.0f);
    unsigned int parts(radius*2);

    unsigned int size(array.getVertexCount());
    array.resize(size+4*parts);

    for(unsigned int i(0) ; i < parts ; i++)
    {
        drawQuad(&array[size+i*4], sf::FloatRect(pos.x-radius, pos.y-radius+i, radius*2, 1), sf::FloatRect(70, i, radius*2, 1));

        for(unsigned int j(0) ; j < 4 ; j++)
            array[size+4*i+j].color.a = alpha;

        float offset(waveHeight*std::sin(wavePos+i*waveWidth));
        float offset1(waveHeight*std::sin(wavePos+(i+1)*waveWidth));
        array[size+4*i].position.x += offset;
        array[size+4*i+1].position.x += offset;
        array[size+4*i+2].position.x += offset1;
        array[size+4*i+3].position.x += offset1;
    }
}

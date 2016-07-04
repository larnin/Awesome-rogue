#ifndef MOBSPAWN_H
#define MOBSPAWN_H

#include "particule.h"
#include "Utilities/ressource.h"
#include <vector>

class MobSpawn : public Particule
{
public:
    MobSpawn(const Location & pos, float timeToSpawn);
    MobSpawn(MobSpawn &&) = default;
    MobSpawn & operator =(MobSpawn &&) = default;
    virtual ~MobSpawn() = default;

    virtual void update(const sf::Time & elapsedTime);
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    void updateIncant(const sf::Time & elapsedTime);

    struct IncantPart
    {
        IncantPart(float _alpha, const sf::Vector2f & _offset)
            : lifeTime(0)
            , alpha(_alpha)
            , maxAlpha(_alpha)
            , offsetSpeed(_offset)
            , offset(0, 0)
        {

        }

        float lifeTime;
        float alpha;
        float maxAlpha;
        sf::Vector2f offsetSpeed;
        sf::Vector2f offset;
    };

    float m_totalTime;
    float m_lifeTime;
    float m_timeFromLastIncantSpawn;
    Texture m_incantTexture;
    Texture m_lightTexture;
    std::vector<IncantPart> m_incantAura;


};

#endif // MOBSPAWN_H

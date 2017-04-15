#ifndef MOBDEATH_H
#define MOBDEATH_H

#include "particule.h"
#include "Utilities/ressource.h"
#include <SFML/Graphics/Rect.hpp>

class MobDeath : public Particule
{
public:
    MobDeath(const Location & pos, const Texture & texture, const sf::FloatRect & rect, float angle, const sf::Vector2f & speed);
    MobDeath(MobDeath &&) = default;
    MobDeath & operator =(MobDeath &&) = default;
    virtual ~MobDeath() = default;

    virtual void update(const sf::Time & elapsedTime) override;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;

private:
    struct Part
    {
        Part(float _orientation, const sf::Vector2f & _offset, const sf::FloatRect & _texRect
             , const sf::Vector2f _speed, float _rotationSpeed, float _maxLifeTime)
            : orientation(_orientation)
            , offset(_offset)
            , texRect(_texRect)
            , speed(_speed)
            , rotationSpeed(_rotationSpeed)
            , maxLifeTime(_maxLifeTime)
            , alpha(255)
        {}

        float orientation;
        sf::Vector2f offset;
        sf::FloatRect texRect;
        sf::Vector2f speed;
        float rotationSpeed;
        float maxLifeTime;
        int alpha;
    };

    void generateParts(const sf::FloatRect & rect, float angle, const sf::Vector2f & speed);

    Texture m_texture;
    float m_lifeTime;
    std::vector<Part> m_parts;
};

#endif // MOBDEATH_H

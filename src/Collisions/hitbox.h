#ifndef HITBOX_H
#define HITBOX_H

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/VertexArray.hpp>

struct Line
{
    Line(const sf::Vector2f & _pos1, const sf::Vector2f & _pos2)
        : pos1(_pos1), pos2(_pos2) {}
    sf::Vector2f pos1;
    sf::Vector2f pos2;
    Line move(const sf::Vector2f & delta) const;
    bool sameAs(const Line & l, float epsilon);
};

class HitBox
{
    friend class Collisions;

public:
    HitBox() = default;
    ~HitBox() = default;
    void addLine(const Line & l);
    sf::FloatRect globalRect() const;
    HitBox transform(float rotation, bool xFlip, bool yFlip) const;
    HitBox transform(const sf::Vector2f & dir) const;

private:
    std::vector<Line> m_lines;
};

#endif // HITBOX_H

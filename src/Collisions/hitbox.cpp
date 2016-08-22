#include "hitbox.h"
#include "Utilities/vect2convert.h"

Line Line::move(const sf::Vector2f & delta) const
{
    return Line(pos1+delta, pos2+delta);
}

bool Line::sameAs(const Line & l, float epsilon)
{
    if(epsilon < 0)
        return false;
    if(norm(l.pos1-pos1) < epsilon && norm(l.pos2-pos2) < epsilon)
        return true;
    if(norm(l.pos1-pos2) < epsilon && norm(l.pos2-pos1) < epsilon)
        return true;
    return false;
}

void HitBox::addLine(const Line & l)
{
    m_lines.push_back(l);
}

sf::FloatRect HitBox::globalRect() const
{
    const float epsilon(0.01f);
    sf::FloatRect rect;
    bool sizeCreated(false);

    for(auto & line : m_lines)
    {
        if(!sizeCreated)
        {
            sizeCreated = true;
            rect = sf::FloatRect(sf::Vector2f(std::min(line.pos1.x, line.pos2.x), std::min(line.pos1.y, line.pos2.y))
                            , sf::Vector2f(std::abs(line.pos1.x-line.pos2.x), std::abs(line.pos1.y-line.pos2.y)));
            continue;
        }
        float xMin(std::min(std::min(line.pos1.x, line.pos2.x), rect.left));
        float yMin(std::min(std::min(line.pos1.y, line.pos2.y), rect.top));
        float xMax(std::max(std::max(line.pos1.x, line.pos2.x), rect.left+rect.width));
        float yMax(std::max(std::max(line.pos1.y, line.pos2.y), rect.top+rect.height));
        rect = sf::FloatRect(xMin, yMin, xMax-xMin, yMax-yMin);
    }
    rect.left -= epsilon;
    rect.top -= epsilon;
    rect.width += 2*epsilon;
    rect.height += 2*epsilon;
    return rect;
}

HitBox HitBox::transform(float rotation, bool xFlip, bool yFlip) const
{
    HitBox out;

    for(const auto & line : m_lines)
    {
        Line l(rotate(line.pos1, rotation), rotate(line.pos2, rotation));
        if(xFlip)
        {
            l.pos1.x *= -1;
            l.pos2.x *= -1;
        }
        if(yFlip)
        {
            l.pos1.y *= -1;
            l.pos2.y *= -1;
        }
        out.addLine(l);
    }
    return out;
}

HitBox HitBox::transform(const sf::Vector2f & dir) const
{
    HitBox out;

    for(const auto & line : m_lines)
        out.addLine(line.move(dir));
    return out;
}

HitBox HitBox::transform(float factor)
{
    HitBox out;
    for(const auto & line : m_lines)
        out.addLine(Line(line.pos1*factor, line.pos2*factor));
    return out;
}

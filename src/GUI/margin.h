#ifndef MARGIN_H
#define MARGIN_H

#include <SFML/Graphics/Rect.hpp>

struct Margin
{
    Margin(float _left, float _right, float _top, float _bottom)
        : top(_top), bottom(_bottom), left(_left), right(_right)
    {}
    Margin(float x, float y)
        : top(y), bottom(y), left(x), right(x)
    {}
    Margin(float value)
        : top(value), bottom(value), left(value), right(value)
    {}

    Margin operator+(const Margin & m) const
    {
        return Margin(left+m.left, right+m.right, top+m.top, bottom+m.bottom);
    }

    Margin& operator+=(const Margin & m)
    {
        left += m.left;
        right += m.right;
        top += m.top;
        bottom += m.bottom;
        return *this;
    }

    Margin operator-(const Margin & m) const
    {
        return Margin(left-m.left, right-m.right, top-m.top, bottom-m.bottom);
    }

    Margin& operator-=(const Margin & m)
    {
        left -= m.left;
        right -= m.right;
        top -= m.top;
        bottom -= m.bottom;
        return *this;
    }

    static sf::FloatRect applyMargin(const sf::FloatRect & rect, const Margin & m)
    {
        return sf::FloatRect(rect.left + m.left, rect.top + m.top, rect.width-m.left-m.right, rect.height-m.top-m.bottom);
    }

    float top;
    float bottom;
    float left;
    float right;
};


enum VAlign
{
    V_TOP,
    V_CENTER,
    V_BOTTOM
};

enum HAlign
{
    H_LEFT,
    H_CENTER,
    H_RIGHT
};

#endif // MARGIN_H

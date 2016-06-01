#include "adaptablebounds.h"

AdaptableBounds::AdaptableBounds()
    : pos(0, 0)
    , size(1, 1)
    , margin(0)
    , vAlign(VAlign::V_TOP)
    , hAlign(HAlign::H_LEFT)
{

}

AdaptableBounds::AdaptableBounds(const sf::Vector2f & _pos
                , const sf::Vector2f & _size
                , const Margin & _margin
                , VAlign _vAlign
                , HAlign _hAlign)
    : pos(_pos)
    , size(_size)
    , margin(_margin)
    , vAlign(_vAlign)
    , hAlign(_hAlign)
{

}

sf::FloatRect AdaptableBounds::boundsOnView(const sf::View & v) const
{
    sf::Vector2f widgetSize(size);
    sf::Vector2f widgetPos(pos);
    widgetPos += v.getCenter();

    switch(hAlign)
    {
    case H_LEFT:
    default:
        widgetPos.x -= v.getSize().x/2.0f;
    break;
    case H_CENTER:
        widgetPos.x -= widgetSize.x/2.0f;
    break;
    case H_RIGHT:
        widgetPos.x += v.getSize().x/2.0f - widgetSize.x;
    break;
    }

    switch(vAlign)
    {
    case V_TOP:
    default:
        widgetPos.y -= v.getSize().y/2.0f;
    break;
    case V_CENTER:
        widgetPos.y -= widgetSize.y/2.0f;
    break;
    case V_BOTTOM:
        widgetPos.y += v.getSize().y/2.0f - widgetSize.y;
    break;
    }

    widgetPos.x += margin.left;
    widgetPos.y += margin.top;
    widgetSize.x -= margin.left+margin.right;
    widgetSize.y -= margin.top+margin.bottom;

    return sf::FloatRect(widgetPos, widgetSize);
}

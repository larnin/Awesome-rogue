#include "graphichelper.h"
#include "Utilities/quadrender.h"
#include <SFML/Graphics/Text.hpp>

#include "GUI/margin.h"

sf::VertexArray drawSelector(sf::FloatRect rect)
{
    rect.left--;
    rect.top--;
    rect.width += 2;
    rect.height += 2;

    const sf::Color c(sf::Color::Black);
    const float distLines(15);
    unsigned int nbLineX(rect.width/distLines-1);
    unsigned int nbLineY(rect.height/distLines-1);
    float offsetX((rect.width-(nbLineX-1)*distLines)/2);
    float offsetY((rect.height-(nbLineY-1)*distLines)/2);

    sf::VertexArray array(sf::Quads, 32 + 8*(nbLineX+nbLineY));

    drawFlatQuad(&array[0], sf::FloatRect(rect.left, rect.top, 3, 1), c);
    drawFlatQuad(&array[4], sf::FloatRect(rect.left, rect.top+1, 1, 2), c);

    drawFlatQuad(&array[8], sf::FloatRect(rect.left+rect.width-3, rect.top, 3, 1), c);
    drawFlatQuad(&array[12], sf::FloatRect(rect.left+rect.width-1, rect.top+1, 1, 2), c);

    drawFlatQuad(&array[16], sf::FloatRect(rect.left, rect.top+rect.height-1, 3, 1), c);
    drawFlatQuad(&array[20], sf::FloatRect(rect.left, rect.top+rect.height-3, 1, 2), c);

    drawFlatQuad(&array[24], sf::FloatRect(rect.left+rect.width-3, rect.top+rect.height-1, 3, 1), c);
    drawFlatQuad(&array[28], sf::FloatRect(rect.left+rect.width-1, rect.top+rect.height-3, 1, 2), c);

    unsigned int offsetRender(32);

    for(unsigned int i(0) ; i < nbLineX ; i++)
    {
        drawFlatQuad(&array[offsetRender+8*i], sf::FloatRect(rect.left + offsetX + i*distLines - 1, rect.top, 3, 1), c);
        drawFlatQuad(&array[offsetRender+8*i+4], sf::FloatRect(rect.left + offsetX + i*distLines - 1, rect.top+rect.height-1, 3, 1), c);
    }

    offsetRender+= 8*nbLineX;
    for(unsigned int i(0) ; i < nbLineY ; i++)
    {
        drawFlatQuad(&array[offsetRender+8*i], sf::FloatRect(rect.left , rect.top + offsetY + i*distLines - 1, 1, 3), c);
        drawFlatQuad(&array[offsetRender+8*i+4], sf::FloatRect(rect.left+rect.width-1 , rect.top + offsetY + i*distLines - 1, 1, 3), c);
    }
    return array;
}

sf::VertexArray drawButton(const sf::FloatRect & rect, const sf::FloatRect & texRect, const Margin & border)
{
    sf::VertexArray array(sf::Quads, 36);
    //corners
    drawQuad(&array[0], sf::FloatRect(rect.left, rect.top, border.left, border.top)
            , sf::FloatRect(texRect.left, texRect.top, border.left, border.top));
    drawQuad(&array[4], sf::FloatRect(rect.left+rect.width-border.right, rect.top, border.right, border.top)
            , sf::FloatRect(texRect.left+texRect.width-border.right, texRect.top, border.right, border.top));
    drawQuad(&array[8], sf::FloatRect(rect.left, rect.top+rect.height-border.bottom, border.left, border.bottom)
            , sf::FloatRect(texRect.left, texRect.top+texRect.height-border.bottom, border.left, border.bottom));
    drawQuad(&array[12], sf::FloatRect(rect.left+rect.width-border.right, rect.top+rect.height-border.bottom, border.right, border.bottom)
            , sf::FloatRect(texRect.left+texRect.width-border.right, texRect.top+texRect.height-border.bottom, border.right, border.bottom));
    //borders
    drawQuad(&array[16], sf::FloatRect(rect.left+border.left, rect.top, rect.width-border.left-border.right, border.top)
            , sf::FloatRect(texRect.left+border.left, texRect.top, texRect.width-border.left-border.right, border.top));
    drawQuad(&array[20], sf::FloatRect(rect.left+border.left, rect.top+rect.height-border.bottom, rect.width-border.left-border.right, border.bottom)
            , sf::FloatRect(texRect.left+border.left, texRect.top+texRect.height-border.bottom, texRect.width-border.left-border.right, border.bottom));
    drawQuad(&array[24], sf::FloatRect(rect.left, rect.top+border.top, border.left, rect.height-border.top-border.bottom)
            , sf::FloatRect(texRect.left, texRect.top+border.top, border.left, texRect.height-border.top-border.bottom));
    drawQuad(&array[28], sf::FloatRect(rect.left+rect.width-border.right, rect.top+border.top, border.right, rect.height-border.top-border.bottom)
            , sf::FloatRect(texRect.left+texRect.width-border.right, texRect.top+border.top, border.right, texRect.height-border.top-border.bottom));
    //center
    drawQuad(&array[32], sf::FloatRect(rect.left+border.left, rect.top+border.top, rect.width-border.left-border.right, rect.height-border.top-border.bottom)
            , sf::FloatRect(texRect.left+border.left, texRect.top+border.top, texRect.width-border.left-border.right, texRect.height-border.top-border.bottom));
    return array;
}

sf::Texture drawText(const std::string & _text, const sf::Font & font, const sf::Vector2f & size, VAlign v, HAlign h)
{
    sf::RenderTexture texture;
    texture.create(size.x, size.y);

    sf::Text text(_text, font, 8);
    text.setColor(sf::Color::Black);
    text.setPosition(-text.getGlobalBounds().left, -text.getGlobalBounds().top);

    sf::Vector2f offset;
    if(v == VAlign::V_CENTER)
        offset.y = int((texture.getSize().y - text.getGlobalBounds().height)/2.0f);
    if(v == VAlign::V_BOTTOM)
        offset.y = texture.getSize().y - text.getGlobalBounds().height;
    if(h == HAlign::H_CENTER)
        offset.x = int((texture.getSize().x - text.getGlobalBounds().width)/2.0f);
    if(h == HAlign::H_RIGHT)
        offset.x = texture.getSize().x - text.getGlobalBounds().width;

    text.move(offset);

    texture.clear(sf::Color(0, 0, 0, 0));
    texture.draw(text);
    texture.display();

    return texture.getTexture();
}

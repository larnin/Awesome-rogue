#include "showfps.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

ShowFPS::ShowFPS()
    : m_time(0)
    , m_frameCount(0)
    , m_font("res/font/PressStart2P.ttf")
    , m_text(VAlign::V_TOP, HAlign::H_LEFT)

{
    m_text.setText("0", m_font, 8, sf::Color(220, 220, 220));
}

void ShowFPS::update(const sf::Time & elapsedTime)
{
    m_time += elapsedTime.asSeconds();
    m_frameCount++;

    if(m_time >= 1)
    {
        m_text.setText(std::to_string(m_frameCount));
        m_time = 0;
        m_frameCount = 0;
    }
}

void ShowFPS::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    sf::View v(target.getView());
    sf::Vector2f size(v.getSize());
    sf::Vector2f topLeft(v.getCenter()-size/2.0f);

    m_text.draw(target, sf::FloatRect(topLeft+sf::Vector2f(5, 5), sf::Vector2f(100, 20)));
}

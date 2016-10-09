#include "lightrender.h"
#include "Types/light.h"
#include "Events/Datas/eventchangelightcolor.h"
#include "Events/Datas/eventaddlight.h"
#include "Events/Datas/eventdellight.h"
#include <SFML/Graphics/Sprite.hpp>

LightRender::LightRender()
    : m_baseColor(sf::Color::White)
    , m_saturedColor(sf::Color::Black)
{
    m_render = std::make_unique<sf::RenderTexture>();

    connect<EventChangeLightColor>(std::bind(&onColorChange, this, _1));
    connect<EventAddLight>(std::bind(&onAddLight, this, _1));
    connect<EventDelLight>(std::bind(&onDelLight, this, _1));
}

void LightRender::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    if(m_render->getSize() != sf::Vector2u(target.getView().getSize()))
        m_render->create(target.getView().getSize().x, target.getView().getSize().y);

    sf::View v(m_render->getView());
    v.setCenter(target.getView().getCenter());
    m_render->setView(v);

    m_render->clear(m_baseColor);

    for(const auto & l : m_lights)
    {
        auto lLock(l.lock());
        if(lLock)
            lLock->drawBase(*m_render);
    }

    m_render->display();

    sf::Sprite s(m_render->getTexture());
    s.move(target.getView().getCenter()-target.getView().getSize()/2.0f);
    target.draw(s, sf::RenderStates(sf::BlendMultiply));

    m_render->clear(m_saturedColor);

    for(const auto & l : m_lights)
    {
        auto lLock(l.lock());
        if(lLock)
            lLock->drawSaturation(*m_render);
    }

    m_render->display();
    target.draw(s, sf::RenderStates(sf::BlendAdd));

    auto it(std::remove_if(m_lights.begin(), m_lights.end(), [](const auto & l){auto lLock(l.lock()); return !lLock;}));
    m_lights.erase(it, m_lights.end());
}

void LightRender::setColors(const sf::Color & base, const sf::Color & saturated)
{
    m_baseColor = base;
    m_saturedColor = saturated;
}

void LightRender::addLight(const std::weak_ptr<Light> & l)
{
    std::shared_ptr<Light> lLock(l.lock());
    if(!lLock)
        return;

    if(std::find_if(m_lights.begin(), m_lights.end(), [lLock](const auto & lItem){
        auto lItemLock(lItem.lock());
        return lItemLock == lLock;
    }) != m_lights.end())
        return;

    m_lights.push_back(l);
}

void LightRender::delLight(const std::weak_ptr<Light> & l)
{
    std::shared_ptr<Light> lLock(l.lock());
    if(!lLock)
        return;

    auto it(std::find_if(m_lights.begin(), m_lights.end(), [lLock](const auto & lItem){
        auto lItemLock(lItem.lock());
        return lItemLock == lLock;
    }));
    if(it == m_lights.end())
        return;
    std::swap(*it, m_lights.back());
    m_lights.pop_back();
}

void LightRender::onColorChange(EventChangeLightColor e)
{
    setColors(e.base, e.saturated);
}

void LightRender::onAddLight(EventAddLight e)
{
    addLight(e.light);
}

void LightRender::onDelLight(EventDelLight e)
{
    delLight(e.light);
}

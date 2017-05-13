#include "lightholder.h"
#include "phongshader.h"
#include "Events/Datas/eventsetambiantcolor.h"
#include <algorithm>

LightHolder::LightHolder()
{
    setAmbiant(sf::Color::White);
    connect<EventSetAmbiantColor>(std::bind(&onAmbiantChange, this, _1));
}

void LightHolder::setAmbiant(const sf::Color & ambiant)
{
    m_ambiantColor = ambiant;
    PhongShader::setAmbiant(m_ambiantColor);
}

void LightHolder::addPointLight(std::weak_ptr<PointLight> light)
{
    auto l = light.lock();
    if(!l)
        return;
    if(std::find_if(m_pointLights.begin(), m_pointLights.end(), [l](const auto & item){return l == item.lock();}) != m_pointLights.end())
        return;

    m_pointLights.push_back(light);
}

void LightHolder::delPointLight(std::weak_ptr<PointLight> light)
{
    auto l = light.lock();
    if(!l)
        return;
    auto it(std::find_if(m_pointLights.begin(), m_pointLights.end(), [l](const auto & item){return l == item.lock();}));
    if(it == m_pointLights.end())
        return;
    std::swap(*it, m_pointLights.back());
    m_pointLights.pop_back();
}

void LightHolder::update(const sf::Time &)
{
    updatePointLights();
}

void LightHolder::updatePointLights()
{
    std::vector<sf::Glsl::Vec3> positions;
    std::vector<sf::Glsl::Vec4> colors;
    std::vector<float> radius;

    for(const auto & l : m_pointLights)
    {
        auto light(l.lock());
        if(!light)
            continue;
        positions.push_back(light->pos);
        colors.push_back(light->color);
        radius.push_back(light->radius);
    }
    PhongShader::setPointLights(positions, colors, radius);

    m_pointLights.erase(std::remove_if(m_pointLights.begin(), m_pointLights.end(), [](const auto & l){return l.expired();}), m_pointLights.end());
}

void LightHolder::onAmbiantChange(EventSetAmbiantColor e)
{
    setAmbiant(e.color);
}

void LightHolder::onAddPointLight(EventAddPointLight e)
{
    addPointLight(e.light);
}

void LightHolder::onDelPointLight(EventDelPointLight e)
{
    delPointLight(e.light);
}

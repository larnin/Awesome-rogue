#include "lightholder.h"
#include "phongshader.h"
#include "Events/Datas/Light/eventsetambiantcolor.h"
#include <algorithm>

LightHolder::LightHolder()
{
    setAmbiant(sf::Color::White);
    connect<EventSetAmbiantColor>(std::bind(&onAmbiantChange, this, _1));
    connect<EventAddLight>(std::bind(&onAddLight, this, _1));
    connect<EventDelLight>(std::bind(&onDelLight, this, _1));
}

void LightHolder::setAmbiant(const sf::Color & ambiant)
{
    m_ambiantColor = ambiant;
    PhongShader::setAmbiant(m_ambiantColor);
}

void LightHolder::addLight(std::weak_ptr<LightData> light)
{
    auto l = light.lock();
    if(!l)
        return;
    if(std::find_if(m_lights.begin(), m_lights.end(), [l](const auto & item){return l == item.lock();}) != m_lights.end())
        return;

    m_lights.push_back(light);
}

void LightHolder::delLight(std::weak_ptr<LightData> light)
{
    auto l = light.lock();
    if(!l)
        return;
    auto it(std::find_if(m_lights.begin(), m_lights.end(), [l](const auto & item){return l == item.lock();}));
    if(it == m_lights.end())
        return;
    std::swap(*it, m_lights.back());
    m_lights.pop_back();
}

void LightHolder::update(const sf::Time &)
{
    updateLights();
}

void LightHolder::updateLights()
{
    std::vector<sf::Glsl::Vec4> lightsColor;
    std::vector<sf::Glsl::Vec3> lightsPos;
    std::vector<float> lightsType;
    std::vector<sf::Glsl::Vec4> lightsParams;

    for(const auto & l : m_lights)
    {
        auto light(l.lock());
        if(!light)
            continue;

        lightsColor.push_back(light->color);
        lightsPos.push_back(light->pos);
        lightsType.push_back(light->type);
        lightsParams.push_back(sf::Glsl::Vec4(light->radius, light->intensity, light->yaw, light->pitch));
    }

    PhongShader::setLights(lightsColor, lightsPos, lightsType, lightsParams);

    m_lights.erase(std::remove_if(m_lights.begin(), m_lights.end(), [](const auto & l){return l.expired();}), m_lights.end());
}

void LightHolder::onAmbiantChange(EventSetAmbiantColor e)
{
    setAmbiant(e.color);
}

void LightHolder::onAddLight(EventAddLight e)
{
    addLight(e.light);
}

void LightHolder::onDelLight(EventDelLight e)
{
    delLight(e.light);
}

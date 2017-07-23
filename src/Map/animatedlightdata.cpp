#include "animatedlightdata.h"
#include <algorithm>
#include <cmath>

LightFrame::LightFrame(float _time)
    : time(_time)
    , pos(0, 0, 100)
    , color(sf::Color::White)
    , radius(100)
    , yaw(0)
    , pitch(0)
    , intensity(1)
{

}

LightFrame::LightFrame(const json & obj)
    : time(obj["time"].get<float>())
    , radius(obj["radius"].get<float>())
    , yaw(obj["yaw"].get<float>())
    , pitch(obj["pitch"].get<float>())
    , intensity(obj["intensity"].get<float>())
{
    auto& posObj(obj["pos"]);
    pos = sf::Vector3f(posObj["x"].get<float>(), posObj["y"].get<float>(), posObj["z"].get<float>());

    auto& colorObj(obj["color"]);
    color = sf::Color(colorObj["r"].get<int>(), colorObj["g"].get<int>(), colorObj["b"].get<int>(), colorObj["a"].get<int>());
}

json LightFrame::toJson() const
{
    json obj;
    obj["time"] = time;
    obj["radius"] = radius;
    obj["yaw"] = yaw;
    obj["pitch"] = pitch;
    obj["intensity"] = intensity;
    json posObj;
    posObj["x"] = pos.x;
    posObj["y"] = pos.y;
    posObj["z"] = pos.z;
    obj["pos"] = posObj;
    json colorObj;
    colorObj["r"] = color.r;
    colorObj["g"] = color.g;
    colorObj["b"] = color.b;
    colorObj["a"] = color.a;
    obj["color"] = colorObj;
    return obj;
}

AnimatedLightData::AnimatedLightData(LightType type)
    : m_type(type)
{

}

AnimatedLightData::AnimatedLightData(const json & obj)
    : m_type(LightType(obj["type"].get<int>()))
{
    auto array(obj["frames"]);
    for(const auto & v : array)
        m_frames.emplace_back(v);
}

json AnimatedLightData::toJson() const
{
    json obj;
    obj["type"] = int(m_type);
    json array;
    for(const auto & v : m_frames)
        array.push_back(v.toJson());
    obj["frames"] = array;
    return obj;
}

float AnimatedLightData::time() const
{
    return std::accumulate(m_frames.begin(), m_frames.end(), 0.0f, [](float a, const LightFrame & b){return a + b.time;});
}

void AnimatedLightData::add(const LightFrame & f)
{
    m_frames.push_back(f);
}

void AnimatedLightData::del(unsigned int index)
{
    if(index >= m_frames.size())
        return;
    m_frames.erase(std::next(m_frames.begin(), index));
}

LightFrame AnimatedLightData::at(float t) const
{
    if(m_frames.empty())
        return defaultFrame();
    t = fmod(t, time());

    auto index = frameIndex(t);
    auto nextIndex = index >= m_frames.size()-1 ? 0 : index + 1;
    return interpolate(m_frames[index], m_frames[nextIndex], t / m_frames[index].time);
}

LightFrame AnimatedLightData::defaultFrame()
{
    return LightFrame(1);
}

unsigned int AnimatedLightData::frameIndex(float & t) const
{
    float totalTime(0);
    for(unsigned int i(0) ; i < m_frames.size() ; i++)
    {
        totalTime += m_frames[i].time;
        if(totalTime >= t)
        {
            t = t - totalTime + m_frames[i].time;
            return i;
        }
    }

    t = 0;
    return m_frames.size() - 1;
}

LightFrame AnimatedLightData::interpolate(const LightFrame & f1, const LightFrame &f2, float lerp)
{
    LightFrame f(f1.time * lerp);
    f.color = sf::Color(interpolate(f1.color.r, f2.color.r, lerp)
                      , interpolate(f1.color.g, f2.color.g, lerp)
                      , interpolate(f1.color.b, f2.color.b, lerp)
                      , interpolate(f1.color.a, f2.color.a, lerp));
    f.pitch = interpolate(f1.pitch, f2.pitch, lerp);
    f.pos = sf::Vector3f(interpolate(f1.pos.x, f2.pos.x, lerp)
                       , interpolate(f1.pos.y, f2.pos.y, lerp)
                       , interpolate(f1.pos.z, f2.pos.z, lerp));
    f.radius = interpolate(f1.radius, f2.radius, lerp);
    f.yaw = interpolate(f1.yaw, f2.yaw, lerp);
    f.intensity = interpolate(f1.intensity, f2.intensity, lerp);
    return f;
}


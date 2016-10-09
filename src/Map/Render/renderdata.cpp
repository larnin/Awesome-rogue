#include "renderdata.h"
#include "Libs/json.hpp"
#include <algorithm>
#include <numeric>
#include <fstream>

std::default_random_engine RenderData::m_rand;
const std::string dir("res/img/tileset/");

RenderData::RenderData(const std::string & filename)
{
    using json = nlohmann::json;

    std::ifstream file(dir+filename+".json");
    if(!file.is_open())
        return;
    json j(json::parse(file));
    file.close();

    if(!j.is_object())
        return;

    texture.load(dir+j["texture"].get<std::string>());

    json jBlocks(j["blocks"]);
    for(const json & jB : jBlocks)
    {
        BlockAnimation anim(jB["id"].get<unsigned int>());
        for(const json & jAnim : jB["anims"])
        {
            SingleBlockAnimation a(jAnim["weight"].get<float>());
            for(const json jFrame : jAnim["frames"])
            {
                float mintime(jFrame["mintime"]);
                float maxtime(jFrame["maxtime"]);
                a.frames.push_back(BlockFrame(std::min(mintime, maxtime), std::max(mintime, maxtime)
                                              , jFrame["id"].get<unsigned int>(), Rotation(jFrame["rot"].get<int>())
                                              , jFrame["flipx"], jFrame["flipy"]));
            }
            if(!a.frames.empty())
                anim.anims.push_back(a);
        }
        if(!anim.anims.empty())
            m_animations.push_back(anim);
    }

    initAnimation();
}

void RenderData::initAnimation()
{
    for(const BlockAnimation & a : m_animations)
    {
        assert(!a.anims.empty());
        m_currentAnimation.emplace_back(a.id, create(a.id));
    }

    for(const auto & a : m_currentAnimation)
    {
        assert(!a.second.anim.frames.empty());
        m_currentFrame.emplace_back(a.first, a.second.anim.frames.front());
    }
}

bool RenderData::hasAnimation(unsigned int id) const
{
    return std::find_if(m_currentFrame.begin(), m_currentFrame.end(), [id](const auto & f){return f.first == id;}) != m_currentFrame.end();
}

void RenderData::addTime(float time)
{
    for(auto & a : m_currentAnimation)
    {
        a.second.time += time;
        if(a.second.finished())
            a.second = create(a.first);

        auto it(std::find_if(m_currentFrame.begin(), m_currentFrame.end(), [a](const auto & f){return f.first == a.first;}));
        assert(it!=m_currentFrame.end());
        it->second = a.second.current();
    }
}

BlockFrame RenderData::frameOf(unsigned int id) const
{
    auto it(std::find_if(m_currentFrame.begin(), m_currentFrame.end(), [id](const auto & f){return f.first == id;}));
    if(it == m_currentFrame.end())
        return BlockFrame(0.1, 0.1, id, ROT_0, false, false);

    return it->second;
}

RenderData::PlayedAnimation RenderData::create(unsigned int id)
{
    auto it(std::find_if(m_animations.begin(), m_animations.end(), [id](const auto & a){return a.id == id;}));
    assert(it!=m_animations.end());

    BlockAnimation &a(*it);
    assert(!a.anims.empty());

    const auto & anim(a.anims[std::uniform_int_distribution<unsigned int>(0, a.anims.size()-1)(m_rand)]);
    SingleBlockAnimation s(1);
    for(const BlockFrame & f : anim.frames)
    {
        float time(std::uniform_real_distribution<float>(f.minTime, f.maxTime)(m_rand));
        s.frames.push_back(BlockFrame(time, time, f.id, f.rot, f.flipX, f.flipY));
    }
    return PlayedAnimation(s);
}

bool RenderData::PlayedAnimation::finished() const
{
    float total(0);
    for(const BlockFrame & f : anim.frames)
        total += f.minTime;
    //float total(std::accumulate(anim.frames.begin(), anim.frames.end(), 0, [](float t, const auto & f){return t+f.minTime;}));
    return total <= time;
}

BlockFrame RenderData::PlayedAnimation::current() const
{
    assert(!anim.frames.empty());
    float t(time);
    for(const BlockFrame & f : anim.frames)
    {
        t-= f.minTime;
        if(t<= 0)
            return f;
    }
    return anim.frames.back();
}

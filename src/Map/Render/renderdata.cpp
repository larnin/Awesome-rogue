#include "renderdata.h"
#include <algorithm>
#include <numeric>
#include <fstream>
#include "Libs/json.hpp"

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
        BlockAnimation anim;
        for(const json & jFrame : jB)
            anim.frames.push_back(BlockFrame(jFrame["id"], jFrame["time"]));
        if(anim.frames.size() >= 2)
            anims.push_back(anim);
    }
}

bool RenderData::hasAnimation(unsigned int id) const
{
    return std::find_if(anims.begin(), anims.end(), [id](const auto & a){return a.id() == id;}) != anims.end();
}

unsigned int RenderData::frameOf(unsigned int id, float time) const
{
    auto it(std::find_if(anims.begin(), anims.end(), [id](const auto & a){return a.id() == id;}));
    if(it == anims.end())
        return id;

    //float totalTime(std::accumulate(it->frames.begin(), it->frames.end(), 0, [](float time, const auto & f){return time+f.time;}));
    float totalTime(0);
    for(const auto & f : it->frames)
        totalTime += f.time;
    time = fmod(time, totalTime);

    for(const auto & f : it->frames)
    {
        time -= f.time;
        if(time <= 0)
            return f.id;
    }
    return it->frames.front().id;
}

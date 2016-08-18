#include "configs.h"
#include "Libs/json.hpp"
#include <fstream>

const std::string Configs::filename("configs.json");
Configs Configs::c(Configs::filename);

Configs::Configs(const std::string & filename)
    : useFullScreen(false)
    , screenSize(1200, 900)
    , zoom(0.5f)
    , musicVolum(100.0f)
    , soundsVolum(100.0f)
{
    load(filename);
}

Configs::~Configs()
{
    save(filename);
}

void Configs::load(const std::string & filename)
{
    using json = nlohmann::json;

    std::ifstream file(filename);
    if(!file.is_open())
        return;
    json j(json::parse(file));
    file.close();

    if(!j.is_object())
        return;

    useFullScreen = j["fullScreen"];
    screenSize.x = j["screenSizeX"];
    screenSize.y = j["screenSizeY"];
    zoom = j["zoom"];
    musicVolum = j["music"];
    soundsVolum = j["sounds"];
}

void Configs::save(const std::string & filename)
{
    using json = nlohmann::json;

    json j =
    {
        {"fullScreen", useFullScreen},
        {"screenSizeX", screenSize.x},
        {"screenSizeY", screenSize.y},
        {"zoom", zoom},
        {"music", musicVolum},
        {"sounds", soundsVolum}

    };

    std::ofstream file(filename);
    if(!file.is_open())
        return;
    file << j.dump();
    file.close();
}

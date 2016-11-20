#include "configs.h"
#include "Libs/json.hpp"
#include <SFML/Window/VideoMode.hpp>
#include <fstream>

const std::string Configs::filename("configs.json");
Configs Configs::c(Configs::filename);

const float widthProportion(0.7f);
const float heightProportion(0.8f);

Configs::Configs(const std::string & filename)
    : useFullScreen(false)
    , screenSize(sf::VideoMode::getDesktopMode().width*widthProportion, sf::VideoMode::getDesktopMode().height*heightProportion)
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

    try
    {
        useFullScreen = j["fullScreen"];
        screenSize.x = j["screenSizeX"];
        screenSize.y = j["screenSizeY"];
        zoom = j["zoom"];
        musicVolum = j["music"];
        soundsVolum = j["sounds"];
        lang = j["lang"];
    }
    catch(const std::exception & e) {}
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
        {"sounds", soundsVolum},
        {"lang", lang}
    };

    std::ofstream file(filename);
    if(!file.is_open())
        return;
    file << j.dump();
    file.close();
}

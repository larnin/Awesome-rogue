#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include "keyconfig.h"
#include "Libs/json.hpp"
#include <fstream>

const std::string KeysConfig::keysFilename("commands.json");

KeyInfo::KeyInfo(sf::Keyboard::Key _key, unsigned int _button)
    : key(_key)
    , useAxis(false)
    , button(_button)
{

}

KeyInfo::KeyInfo(sf::Keyboard::Key _key, sf::Joystick::Axis _axis, bool _axisSide)
    : key(_key)
    , useAxis(true)
    , axis(_axis)
    , axisSide(_axisSide)
{

}


KeysConfig::KeysConfig()
    : useJoystic(true)
    , joysticID(0)
{
    sf::Joystick::update();
    if(useJoystic)
    {
        int id = getFirstConnectedJoystic();
        if(id == -1)
            useJoystic = false;
        else joysticID = id;
    }

    commands[CommandType::MOVE_UP] = KeyInfo(sf::Keyboard::Key::Up, sf::Joystick::Axis::Y, false);
    commands[CommandType::MOVE_DOWN] = KeyInfo(sf::Keyboard::Key::Down, sf::Joystick::Axis::Y, true);
    commands[CommandType::MOVE_LEFT] = KeyInfo(sf::Keyboard::Key::Left, sf::Joystick::Axis::X, false);
    commands[CommandType::MOVE_RIGHT] = KeyInfo(sf::Keyboard::Key::Right, sf::Joystick::Axis::X, true);
    commands[CommandType::SHOOT_UP] = KeyInfo(sf::Keyboard::Key::Z, sf::Joystick::Axis::R, false);
    commands[CommandType::SHOOT_DOWN] = KeyInfo(sf::Keyboard::Key::S, sf::Joystick::Axis::R, true);
    commands[CommandType::SHOOT_LEFT] = KeyInfo(sf::Keyboard::Key::Q, sf::Joystick::Axis::U, false);
    commands[CommandType::SHOOT_RIGHT] = KeyInfo(sf::Keyboard::Key::D, sf::Joystick::Axis::U, true);
    commands[CommandType::KEY_VALID] = KeyInfo(sf::Keyboard::Key::BackSpace, 1);
    commands[CommandType::KEY_CANCEL] = KeyInfo(sf::Keyboard::Key::Escape, 2);
    commands[CommandType::KEY_PAUSE] = KeyInfo(sf::Keyboard::Key::P, 4);
    commands[CommandType::KEY_ACTION] = KeyInfo(sf::Keyboard::Key::BackSpace, 1);
    commands[CommandType::KEY_MAP] = KeyInfo(sf::Keyboard::Key::M, 6);
    commands[CommandType::KEY_INVENTARY] = KeyInfo(sf::Keyboard::Key::I, 7);
    commands[CommandType::KEY_LEFT_PAGE] = KeyInfo(sf::Keyboard::Key::LShift, 6);
    commands[CommandType::KEY_RIGHT_PAGE] = KeyInfo(sf::Keyboard::Key::RShift, 7);
}

KeysConfig::KeysConfig(const std::string & filename)
    : KeysConfig()
{
    using json = nlohmann::json;

    std::ifstream file(filename);
    if(!file.is_open())
        return;
    json j(json::parse(file));
    file.close();

    if(!j.is_object())
        return;

    joysticID = *(j.find("joysticNumber"));
    useJoystic = *(j.find("useJoystic"));

    auto keys(j.find("keys"));
    if(!keys->is_array() || keys->size() != CommandType::COMMANDS_COUNT)
        return;

    for(unsigned int i(0) ; i < CommandType::COMMANDS_COUNT ; i++)
    {
        json key((*keys)[i]);
        if(!key.is_object())
            continue;
        if(key["useAxis"].get<bool>())
            commands[i] = KeyInfo(sf::Keyboard::Key(key["key"].get<int>()), sf::Joystick::Axis(key["axis"].get<int>()), key["axisSide"]);
        else commands[i] = KeyInfo(sf::Keyboard::Key(key["key"].get<int>()), key["button"]);
    }
}

void KeysConfig::save(const std::string & filename) const
{
    using json = nlohmann::json;

    json j =
    {
        {"joysticNumber", joysticID},
        {"useJoystic", useJoystic}
    };

    json keys;
    for(const auto & key : commands)
    {
        json k;
        k["key"] = key.key;
        k["useAxis"] = key.useAxis;
        if(key.useAxis)
        {
            k["axis"] = key.axis;
            k["axisSide"] = key.axisSide;
        }
        else k["button"] = key.button;
        keys.push_back(k);
    }
    j["keys"] = keys;

    std::ofstream file(filename);
    if(!file.is_open())
        return;
    file << j.dump();
    file.close();
}

int getFirstConnectedJoystic()
{
    for(unsigned int i(0) ; i < sf::Joystick::Count ; i++)
        if(sf::Joystick::isConnected(i))
            return i;
    return -1;
}

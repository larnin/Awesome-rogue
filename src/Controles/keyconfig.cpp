#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include "keyconfig.h"

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
    commands[CommandType::KEY_MAP] = KeyInfo(sf::Keyboard::Key::M, 6);
    commands[CommandType::KEY_INVENTARY] = KeyInfo(sf::Keyboard::Key::I, 7);
    commands[CommandType::KEY_LEFT_PAGE] = KeyInfo(sf::Keyboard::Key::LShift, 6);
    commands[CommandType::KEY_RIGHT_PAGE] = KeyInfo(sf::Keyboard::Key::RShift, 7);
}

KeysConfig::KeysConfig(const std::string & filename)
    : KeysConfig()
{
    QFile file(QString::fromStdString(filename));
    if(!file.exists())
        return;
    if(!file.open(QIODevice::ReadOnly))
        return;
    QJsonParseError error;
    QJsonDocument doc(QJsonDocument::fromJson(file.readAll(), &error));
    if(error.error != QJsonParseError::NoError)
        return;
    if(!doc.isObject())
        return;
    QJsonObject configs(doc.object());
    joysticID = configs.value("joysticNumber").toInt();
    useJoystic = configs.value("useJoystic").toBool();
    QJsonArray keysJson(configs.value("keys").toArray());
    if(keysJson.size() != CommandType::COMMANDS_COUNT)
        return;
    for(unsigned int i(0) ; i < CommandType::COMMANDS_COUNT ; i++)
    {
        QJsonObject key(keysJson.at(i).toObject());
        if(key.isEmpty())
            continue;
        if(key.value("useAxis").toBool())
            commands[i] = KeyInfo(sf::Keyboard::Key(key.value("key").toInt())
                                  , sf::Joystick::Axis(key.value("axis").toInt()), key.value("axisSide").toBool());
        else commands[i] = KeyInfo(sf::Keyboard::Key(key.value("key").toInt())
                                   , key.value("button").toInt());
    }
}

void KeysConfig::save(const std::string & filename) const
{
    QJsonObject configs;
    configs.insert("joysticNumber", int(joysticID));
    configs.insert("useJoystic", useJoystic);
    QJsonArray keysJson;
    for(const auto & key : commands)
    {
        QJsonObject keyJson;
        keyJson.insert("key", key.key);
        keyJson.insert("useAxis", key.useAxis);
        if(key.useAxis)
        {
            keyJson.insert("axis", key.axis);
            keyJson.insert("axisSide", key.axisSide);
        }
        else keyJson.insert("button", int(key.button));
        keysJson.append(keyJson);
    }
    configs.insert("keys", keysJson);

    QFile file(QString::fromStdString(filename));
    if(!file.open(QIODevice::WriteOnly))
        return;
    file.write(QJsonDocument(keysJson).toJson(QJsonDocument::Compact));
    file.close();
}

int getFirstConnectedJoystic()
{
    for(unsigned int i(0) ; i < sf::Joystick::Count ; i++)
        if(sf::Joystick::isConnected(i))
            return i;
    return -1;
}

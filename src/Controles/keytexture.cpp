#include "keytexture.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

std::pair<Texture, Texture> texturesOfKey(const KeyInfo & key)
{
    if(key.useAxis)
        return std::make_pair(keyTexture(key.key), joysticAxisTexture(key.axis, key.axisSide));
    return std::make_pair(keyTexture((key.key)), joysticButtonTexture(key.button));
}

Texture keyTexture(sf::Keyboard::Key key)
{
    sf::RenderTexture texture;
    Texture tBack("res/img/keys/key.png");
    texture.create(tBack->getSize().x, tBack->getSize().y);
    texture.clear(sf::Color::Transparent);
    sf::Sprite s(*tBack);
    texture.draw(s);
    Font f("res/font/PressStart2P.ttf");
    sf::Text t(keyName(key), *f, 8);
    t.setColor(sf::Color::Black);
    auto rect(t.getGlobalBounds());
    sf::Vector2f pos(sf::Vector2f(tBack->getSize())/2.0f-sf::Vector2f(rect.left, rect.top)-sf::Vector2f(rect.width, rect.height)/2.0f);
    pos.y -= 5; //kaka !!
    t.setPosition(sf::Vector2f(sf::Vector2i(pos)));
    texture.draw(t);
    texture.display();
    return texture.getTexture();
}

std::string keyName(sf::Keyboard::Key key)
{
    switch(key)
    {
    default:
    case sf::Keyboard::Unknown:
        return "Unknown";
    case sf::Keyboard::A:
        return "A";
    case sf::Keyboard::B:
        return "B";
    case sf::Keyboard::C:
        return "C";
    case sf::Keyboard::D:
        return "D";
    case sf::Keyboard::E:
        return "E";
    case sf::Keyboard::F:
        return "F";
    case sf::Keyboard::G:
        return "G";
    case sf::Keyboard::H:
        return "H";
    case sf::Keyboard::I:
        return "I";
    case sf::Keyboard::J:
        return "J";
    case sf::Keyboard::K:
        return "K";
    case sf::Keyboard::L:
        return "L";
    case sf::Keyboard::M:
        return "M";
    case sf::Keyboard::N:
        return "N";
    case sf::Keyboard::O:
        return "O";
    case sf::Keyboard::P:
        return "P";
    case sf::Keyboard::Q:
        return "Q";
    case sf::Keyboard::R:
        return "R";
    case sf::Keyboard::S:
        return "S";
    case sf::Keyboard::T:
        return "T";
    case sf::Keyboard::U:
        return "U";
    case sf::Keyboard::V:
        return "V";
    case sf::Keyboard::W:
        return "W";
    case sf::Keyboard::X:
        return "X";
    case sf::Keyboard::Y:
        return "Y";
    case sf::Keyboard::Z:
        return "Z";
    case sf::Keyboard::Num0:
        return "0";
    case sf::Keyboard::Num1:
        return "1";
    case sf::Keyboard::Num2:
        return "2";
    case sf::Keyboard::Num3:
        return "3";
    case sf::Keyboard::Num4:
        return "4";
    case sf::Keyboard::Num5:
        return "5";
    case sf::Keyboard::Num6:
        return "6";
    case sf::Keyboard::Num7:
        return "7";
    case sf::Keyboard::Num8:
        return "8";
    case sf::Keyboard::Num9:
        return "9";
    case sf::Keyboard::Escape:
        return "Esc";
    case sf::Keyboard::LControl:
        return "LCtrl";
    case sf::Keyboard::LShift:
        return "LShift";
    case sf::Keyboard::LAlt:
        return "LAlt";
    case sf::Keyboard::LSystem:
        return "LSysm";
    case sf::Keyboard::RControl:
        return "RCtrl";
    case sf::Keyboard::RShift:
        return "RShift";
    case sf::Keyboard::RAlt:
        return "RAlt";
    case sf::Keyboard::RSystem:
        return "RSysm";
    case sf::Keyboard::Menu:
        return "Menu";
    case sf::Keyboard::LBracket:
        return "[";
    case sf::Keyboard::RBracket:
        return "]";
    case sf::Keyboard::SemiColon:
        return ";";
    case sf::Keyboard::Comma:
        return ",";
    case sf::Keyboard::Period:
        return ".";
    case sf::Keyboard::Quote:
        return "\"";
    case sf::Keyboard::Slash:
        return "/";
    case sf::Keyboard::BackSlash:
        return "\\";
    case sf::Keyboard::Tilde:
        return "~";
    case sf::Keyboard::Equal:
        return "=";
    case sf::Keyboard::Dash:
        return "_";
    case sf::Keyboard::Space:
        return "Space";
    case sf::Keyboard::Return:
        return "←";
    case sf::Keyboard::BackSpace:
        return "↵";
    case sf::Keyboard::Tab:
        return "Tab";
    case sf::Keyboard::PageUp:
        return "Pg↑";
    case sf::Keyboard::PageDown:
        return "Pg↓";
    case sf::Keyboard::End:
        return "End";
    case sf::Keyboard::Home:
        return "Home";
    case sf::Keyboard::Insert:
        return "Insert";
    case sf::Keyboard::Delete:
        return "Del";
    case sf::Keyboard::Add:
        return "+";
    case sf::Keyboard::Subtract:
        return "-";
    case sf::Keyboard::Multiply:
        return "*";
    case sf::Keyboard::Divide:
        return "/";
    case sf::Keyboard::Left:
        return "Left";
    case sf::Keyboard::Right:
        return "Right";
    case sf::Keyboard::Up:
        return "Up";
    case sf::Keyboard::Down:
        return "Down";
    case sf::Keyboard::Numpad0:
        return "Num0";
    case sf::Keyboard::Numpad1:
        return "Num1";
    case sf::Keyboard::Numpad2:
        return "Num2";
    case sf::Keyboard::Numpad3:
        return "Num3";
    case sf::Keyboard::Numpad4:
        return "Num4";
    case sf::Keyboard::Numpad5:
        return "Num5";
    case sf::Keyboard::Numpad6:
        return "Num6";
    case sf::Keyboard::Numpad7:
        return "Num7";
    case sf::Keyboard::Numpad8:
        return "Num8";
    case sf::Keyboard::Numpad9:
        return "Num9";
    case sf::Keyboard::F1:
        return "F1";
    case sf::Keyboard::F2:
        return "F2";
    case sf::Keyboard::F3:
        return "F3";
    case sf::Keyboard::F4:
        return "F4";
    case sf::Keyboard::F5:
        return "F5";
    case sf::Keyboard::F6:
        return "F6";
    case sf::Keyboard::F7:
        return "F7";
    case sf::Keyboard::F8:
        return "F8";
    case sf::Keyboard::F9:
        return "F9";
    case sf::Keyboard::F10:
        return "F10";
    case sf::Keyboard::F11:
        return "F11";
    case sf::Keyboard::F12:
        return "F12";
    case sf::Keyboard::F13:
        return "F13";
    case sf::Keyboard::F14:
        return "F14";
    case sf::Keyboard::F15:
        return "F15";
    case sf::Keyboard::Pause:
        return "Pause";
    }
}

Texture joysticButtonTexture(unsigned int button)
{
    switch(button)
    {
    case 0:
        return Texture("res/img/keys/buttonA.png");
    case 1:
        return Texture("res/img/keys/buttonB.png");
    case 2:
        return Texture("res/img/keys/buttonX.png");
    case 3:
        return Texture("res/img/keys/buttonY.png");
    case 4:
        return Texture("res/img/keys/buttonLB.png");
    case 5:
        return Texture("res/img/keys/buttonRB.png");
    case 6:
        return Texture("res/img/keys/buttonSELECT.png");
    case 7:
        return Texture("res/img/keys/buttonSTART.png");
    case 8:
        return Texture("res/img/keys/axeL.png");
    case 9:
        return Texture("res/img/keys/axeR.png");
    default:
        return drawedText(std::to_string(button));
    }
}

Texture joysticAxisTexture(sf::Joystick::Axis axis, bool side)
{
    float rot(0);
    sf::Vector2f pos;
    std::string fileName;

    switch(axis)
    {
    case sf::Joystick::X:
        rot = side ? 90 : 270;
        pos = side ? sf::Vector2f(50, 21) : sf::Vector2f(10, 21);
        fileName = "res/img/keys/axeL.png";
    break;
    case sf::Joystick::Y:
        rot = side ? 180 : 0;
        pos = side ? sf::Vector2f(30, 34) : sf::Vector2f(30, 8);
        fileName = "res/img/keys/axeL.png";
    break;
    case sf::Joystick::Z:
        return side ? Texture("res/img/keys/buttonLT.png")
                    : Texture("res/img/keys/buttonRT.png");
    case sf::Joystick::R:
        rot = side ? 180 : 0;
        pos = side ? sf::Vector2f(30, 34) : sf::Vector2f(30, 8);
        fileName = "res/img/keys/axeR.png";
    break;
    case sf::Joystick::U:
        rot = side ? 90 : 270;
        pos = side ? sf::Vector2f(50, 21) : sf::Vector2f(10, 21);
        fileName = "res/img/keys/axeR.png";
    break;
    case sf::Joystick::V:
        return drawedText(side ? "Axe V +" : "Axe V-");
    case sf::Joystick::PovX:
        rot = side ? 90 : 270;
        pos = side ? sf::Vector2f(50, 29) : sf::Vector2f(10, 29);
        fileName = "res/img/keys/cross.png";
    break;
    case sf::Joystick::PovY:
        rot = side ? 0 : 180;
        pos = side ? sf::Vector2f(29, 10) : sf::Vector2f(29, 50);
        fileName = "res/img/keys/cross.png";
    break;
    default:
        return drawedText("Axe " + std::to_string(axis) + (side ? " +" : " -"));
    }

    sf::RenderTexture texture;
    Texture tBack(fileName);
    texture.create(tBack->getSize().x, tBack->getSize().y);
    texture.clear(sf::Color::Transparent);
    sf::Sprite s(*tBack);
    texture.draw(s);
    Texture tArrow("res/img/keys/arrow.png");
    sf::Sprite sArrow(*tArrow);
    sArrow.setOrigin(sf::Vector2f(tArrow->getSize())/2.0f);
    sArrow.rotate(rot);
    sArrow.move(pos);
    texture.draw(sArrow);
    texture.display();
    return texture.getTexture();
}

Texture drawedText(const std::string & text)
{
    sf::RenderTexture texture;
    Font f("res/font/PressStart2P.ttf");
    sf::Text t(text, *f, 8);
    texture.create(t.getGlobalBounds().width, t.getGlobalBounds().height);
    texture.clear(sf::Color::Transparent);
    t.setColor(sf::Color::Black);
    t.setPosition(-t.getGlobalBounds().left, -t.getGlobalBounds().top);
    texture.draw(t);
    texture.display();
    return texture.getTexture();
}

#include "Machine/statemachine.h"
#include "Machine/States/testwidgetsstate.h"
#include "Machine/States/menustate.h"

#include "GUI/showfps.h"
#include "Systemes/drawablelist.h"

int main()
{
    std::shared_ptr<ShowFPS> s(std::make_shared<ShowFPS>());
    DrawableList::add(s, -1);
    Updatable::add(s);

    KeysConfig config(KeysConfig::keysFilename);
    std::shared_ptr<StateMachine> machine(std::make_shared<StateMachine>(config));

    std::unique_ptr<State> state(std::make_unique<MenuState>(machine));
    machine->setNext(state);
    machine->run();

    return 0;
}

/*
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Controles/keytexture.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 400), "SFML works!");
    window.setFramerateLimit(60);
    Texture lastKeyTexture;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
                lastKeyTexture = keyTexture(event.key.code);
            if (event.type == sf::Event::JoystickButtonPressed)
                lastKeyTexture = joysticButtonTexture(event.joystickButton.button);
            if (event.type == sf::Event::JoystickMoved)
            {
                if(std::abs(event.joystickMove.position)>50)
                    lastKeyTexture = joysticAxisTexture(event.joystickMove.axis, event.joystickMove.position > 0);
            }
        }

        window.clear(sf::Color::White);

        if(lastKeyTexture.isValid())
        {
            sf::Sprite s(*lastKeyTexture);
            s.scale(2, 2);
            window.draw(s);
        }
        window.display();
    }

    return 0;
}
*/

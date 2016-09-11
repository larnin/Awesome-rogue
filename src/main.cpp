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

    std::vector<std::unique_ptr<sf::Music>> m;
    m.push_back(std::make_unique<sf::Music>());

    return 0;
}

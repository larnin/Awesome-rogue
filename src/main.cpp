#include "Machine/statemachine.h"
#include "Machine/States/testwidgetsstate.h"
#include "Machine/States/menustate.h"

int main()
{
    KeysConfig config(KeysConfig::keysFilename);
    std::shared_ptr<StateMachine> machine(std::make_shared<StateMachine>(config));

    std::unique_ptr<State> state(std::make_unique<MenuState>(machine));
    machine->setNext(state);
    machine->run();

    return 0;
}


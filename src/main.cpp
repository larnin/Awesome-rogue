#include "Machine/statemachine.h"
#include "Machine/States/mapteststate.h"
#include "Machine/States/testwidgetsstate.h"
#include "Machine/States/menustate.h"

#include <cassert>

#include "Map/block.h"
#include <iostream>

int main()
{
    KeysConfig config(KeysConfig::keysFilename);
    std::shared_ptr<StateMachine> machine(std::make_shared<StateMachine>(config));

    //std::shared_ptr<State> state(std::make_shared<MapTestState>(machine));
    //std::shared_ptr<State> state(std::make_shared<TestWidgetsState>(machine));
    std::unique_ptr<State> state(std::make_unique<MenuState>(machine));
    machine->setNext(state);
    machine->run();

    return 0;
}

#include "Machine/statemachine.h"
#include "Machine/States/testwidgetsstate.h"
#include "Machine/States/menustate.h"
#include "GUI/showfps.h"
#include "Systemes/drawablelist.h"
#include "Utilities/tr.h"
#include "Lights/phongshader.h"

#include "sprite.h"

int main()
{
    Tr::load();

    PhongShader::initialize();

    std::shared_ptr<ShowFPS> s(std::make_shared<ShowFPS>());
    DrawableList::add(s, -1);
    Updatable::add(s);
    /*Material m;
    m.primaryTexture.load("res/img/bigball.png");
    std::shared_ptr<Sprite> sp(std::make_shared<Sprite>(m));
    DrawableList::add(sp, -1);*/


    KeysConfig config(KeysConfig::keysFilename);
    std::shared_ptr<StateMachine> machine(std::make_shared<StateMachine>(config));

    std::unique_ptr<State> state(std::make_unique<MenuState>(machine));
    machine->setNext(state);
    machine->run();

    Tr::save();
    return 0;
}

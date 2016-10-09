/*#include "Machine/statemachine.h"
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
*/

/*#define _WIN32_WINNT 0x0600
#include <iostream>
#include <windows.h>

int main()
{
    long unsigned int count(0);
    long unsigned int size(0);
    bool ok(GetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &count, nullptr, &size));
    if(!ok)
        return 0;
    wchar_t *cLang = new wchar_t[size];

    ok = GetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &count, cLang, &size);
    if(!ok)
        return 0;
    std::wcout << std::wstring(cLang, size) << std::endl;
}*/

/* Language system LINUX

if(const char* env_p = std::getenv("LANG"))
    std::cout << "Your LANG is: " << env_p << '\n';
*/

#include "Utilities/tr.h"
#include <iostream>

int main()
{
    std::cout << tr("Test") << std::endl;
    std::cout << tr("With $ params ?", 5) << std::endl;
}

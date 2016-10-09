#include "simplecontroler.h"
#include "Controles/commandsvalue.h"
#include "cassert"
#include <algorithm>

SimpleControler::SimpleControler()
    : Controlable(Controlable::ACTIVE)
{

}

void SimpleControler::control(CommandsValue & v)
{
    for(const auto & f : m_functions)
    {
        if(v.isPressEvent(f.first))
        {
            assert(f.second);
            f.second();
        }
    }
}

void SimpleControler::set(CommandType controle, std::function<void()> function)
{
    auto it(std::find_if(m_functions.begin(), m_functions.end(), [controle](const auto & f){return f.first == controle;}));
    if(it == m_functions.end())
    {
        if(function)
            m_functions.push_back(std::make_pair(controle, function));
        return;
    }
    if(!function)
    {
        m_functions.erase(it);
        return;
    }
    it->second = function;
}

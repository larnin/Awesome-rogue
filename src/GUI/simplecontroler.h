#ifndef SIMPLECONTROLER_H
#define SIMPLECONTROLER_H

#include "Systemes/controlable.h"
#include "Utilities/noncopiable.h"
#include "Controles/commandtype.h"
#include <functional>
#include <vector>

class SimpleControler : public Controlable, private NonCopiable
{
public:
    SimpleControler();
    SimpleControler(SimpleControler &&) = default;
    SimpleControler & operator =(SimpleControler &&) = default;
    virtual ~SimpleControler() = default;

    void set(CommandType controle, std::function<void()> function);

    virtual void control(CommandsValue & v);

private:
    std::vector<std::pair<CommandType, std::function<void()>>> m_functions;
};

#endif // SIMPLECONTROLER_H

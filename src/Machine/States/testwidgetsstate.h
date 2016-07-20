#ifndef TESTWIDGETSSTATE_H
#define TESTWIDGETSSTATE_H

#include <memory>
#include <vector>
#include "state.h"

class Widget;

class TestWidgetsState : public State
{
public:
    TestWidgetsState(std::weak_ptr<StateMachine> machine);
    TestWidgetsState(TestWidgetsState&&) = default;
    TestWidgetsState & operator= (TestWidgetsState &&) = default;
    virtual ~TestWidgetsState() = default;

private:
    void add(std::shared_ptr<Widget> w, unsigned int height = 1);
    std::vector<std::shared_ptr<Widget>> m_widgets;
};

#endif // TESTWIDGETSSTATE_H

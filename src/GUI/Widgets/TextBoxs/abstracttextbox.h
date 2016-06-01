#ifndef ABSTRACTTEXTBOX_H
#define ABSTRACTTEXTBOX_H

#include <functional>
#include "GUI/Widgets/widget.h"

class AbstractTextBox : public Widget
{
public:
    AbstractTextBox(const AdaptableBounds & _bounds = AdaptableBounds());

    virtual ~AbstractTextBox() = default;

    virtual void control(CommandsValue & v);

    void connecTextEntredEvent(const std::function<void(std::string)> & function);
    void disconnectTextEntredEvent();

    std::string text;

protected:
    void replaceCursor();

    unsigned int m_cursorIndex;

private:
    std::function<void(std::string)> m_textEntredFunction;
};

#endif // ABSTRACTTEXTBOX_H

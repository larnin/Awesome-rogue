#ifndef ABSTRACTTEXTBOX_H
#define ABSTRACTTEXTBOX_H

#include "GUI/Widgets/widget.h"
#include <functional>

class AbstractTextBox : public Widget
{
public:
    AbstractTextBox(const AdaptableBounds & _bounds = AdaptableBounds());

    virtual ~AbstractTextBox() = default;

    virtual void control(CommandsValue & v) override;

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

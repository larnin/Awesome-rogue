#ifndef RADIOBUTTONS_H
#define RADIOBUTTONS_H

#include <vector>
#include <functional>
#include "GUI/Widgets/widget.h"
#include "GUI/adaptablebounds.h"

template <typename T>
class RadioButtons : public Widget
{
public:
    RadioButtons(bool vertical = true);
    RadioButtons(AdaptableBounds bounds = AdaptableBounds(), bool vertical = true);

    virtual ~RadioButtons() = default;

    virtual void control(CommandsValue & v);
    virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;
    virtual void changeActiveState(ControlState state);

    void clear();
    void addButton(const std::string & label);
    void setCurrent(const std::string & label);

    std::string getCurrent() const;
    const std::vector<std::string> & allLabels() const;

    void connectIndexChangedEvent(const std::function<void(std::string)> & function);
    void disconnectIndexChangedEvent();

protected:
    std::vector<T> m_buttons;
    int m_currentIndex;
    bool m_vertical;

private:
    void replaceCurrent();
    std::function<void(std::string)> m_indexChangedFunction;
};

#include "radiobuttons.tpp"

#endif // RADIOBUTTONS_H

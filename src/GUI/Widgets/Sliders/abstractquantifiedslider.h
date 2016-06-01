#ifndef ABSTRACTQUANTIFIEDSLIDER_H
#define ABSTRACTQUANTIFIEDSLIDER_H

#include <vector>
#include <functional>
#include "GUI/Widgets/widget.h"

class AbstractQuantifiedSlider : public Widget
{
public:
    AbstractQuantifiedSlider(const AdaptableBounds & _bounds = AdaptableBounds(), bool vertical = false);

    virtual ~AbstractQuantifiedSlider() = default;

    virtual void control(CommandsValue & v);

    void connectValueChangedEvent(const std::function<void(std::string)> & function);
    void disconnectValueChangedEvent();

    void addItem(const std::string & item);
    void clear();
    void setCurrentIndex(int index);

    std::string current() const;
    int currentIndex() const;
    std::string at(int index) const;
    const std::vector<std::string> & items() const;

protected:
    std::vector<std::string> m_items;
    int m_index;
    bool m_vertical;

private:
    std::function<void(std::string)> m_valueChangedfunction;
};

#endif // ABSTRACTQUANTIFIEDSLIDER_H

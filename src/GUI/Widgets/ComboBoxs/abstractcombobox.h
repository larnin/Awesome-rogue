#ifndef ABSTRACTCOMBOBOX_H
#define ABSTRACTCOMBOBOX_H

#include "GUI/Widgets/widget.h"
#include <vector>
#include <functional>

class AbstractComboBox : public Widget
{
public:
    AbstractComboBox(const AdaptableBounds & _bounds = AdaptableBounds());
    virtual ~AbstractComboBox() = default;

    virtual void control(CommandsValue & v) override;

    void connectItemSelectedEvent(const std::function<void(std::string)> & function);
    void disconnectItemSelectedEvent();

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

    bool m_isOpened;
    int m_currentIndex;

private:
    std::function<void(std::string)> m_itemSelectedFunction;
};

#endif // ABSTRACTCOMBOBOX_H

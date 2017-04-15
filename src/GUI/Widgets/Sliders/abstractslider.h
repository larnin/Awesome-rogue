#ifndef ABSTRACTSLIDER_H
#define ABSTRACTSLIDER_H

#include "GUI/Widgets/widget.h"
#include <functional>

class AbstractSlider : public Widget
{
public:
    AbstractSlider(const AdaptableBounds & _bounds = AdaptableBounds(), bool vertical = false);

    virtual ~AbstractSlider() = default;

    virtual void control(CommandsValue & v) override;
    virtual void update(const sf::Time & t) override;

    void connectValueChangedEvent(const std::function<void(float)> & function);
    void disconnectValueChangedEvent();

    float min;
    float max;
    float speed;
    float value;

private:
    std::function<void(float)> m_valueChangedFunction;
    bool m_vertical;

    float m_commandValue;
};

#endif // ABSTRACTSLIDER_H

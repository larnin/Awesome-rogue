#ifndef WIDGET_H
#define WIDGET_H

#include "Systemes/controlable.h"
#include "Systemes/updatable.h"
#include "GUI/adaptablebounds.h"
#include "Controles/commandtype.h"
#include <SFML/Graphics/Drawable.hpp>
#include <memory>
#include <map>

class CommandsValue;

class Widget : public sf::Drawable, public Controlable, public Updatable
{
public:
    Widget(const AdaptableBounds & _bounds = AdaptableBounds());

    virtual ~Widget() = default;

    void connect(CommandType type, std::weak_ptr<Widget> widget);
    void disconnect(CommandType type);
    virtual void control(CommandsValue & v) override;
    virtual void update(const sf::Time &) override;

    AdaptableBounds bounds;

private:
    std::map<CommandType, std::weak_ptr<Widget>> m_connexions;
};

#endif // WIDGET_H

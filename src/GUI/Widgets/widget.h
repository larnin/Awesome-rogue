#ifndef WIDGET_H
#define WIDGET_H

#include <memory>
#include <map>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include "Systemes/controlable.h"
#include "Systemes/updatable.h"
#include "Controles/keyconfig.h"
#include "GUI/adaptablebounds.h"

class Widget : public sf::Drawable, public Controlable, public Updatable
{
public:
    Widget(const AdaptableBounds & _bounds = AdaptableBounds());

    virtual ~Widget() = default;

    void connect(CommandType type, std::weak_ptr<Widget> widget);
    virtual void control(CommandsValue & v);
    virtual void update(const sf::Time &);

    AdaptableBounds bounds;

private:
    std::map<CommandType, std::weak_ptr<Widget>> m_connexions;
};

#endif // WIDGET_H

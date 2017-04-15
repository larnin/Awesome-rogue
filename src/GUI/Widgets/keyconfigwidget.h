#ifndef KEYCONFIGWIDGET_H
#define KEYCONFIGWIDGET_H

#include "widget.h"
#include "Controles/keyconfig.h"
#include "Controles/commandtype.h"
#include "Utilities/ressource.h"
#include <functional>

class KeyConfigWidget : public Widget
{
public:
    KeyConfigWidget(CommandType type, const KeyInfo & currentKey, const AdaptableBounds & _bounds = AdaptableBounds());

    virtual ~KeyConfigWidget() = default;

    virtual void control(CommandsValue & v) override;
    virtual void update(const sf::Time & t) override;
    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const override;

    KeyInfo getKey() const {return m_key;}
    CommandType type() const {return m_type;}

    void connectKeyChanged(std::function<void(CommandType, KeyInfo)> function)
    { m_function = function; }

private:
    KeyInfo m_key;
    CommandType m_type;
    std::function<void(CommandType, KeyInfo)> m_function;
    Texture m_keyTexture;
    Texture m_joyKeyTexture;

    bool m_waitInput;
};

#endif // KEYCONFIGWIDGET_H

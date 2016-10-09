#ifndef LIGHTRENDER_H
#define LIGHTRENDER_H

#include "Utilities/noncopiable.h"
#include "Events/eventreceiver.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <memory>
#include <vector>

class Light;
class EventChangeLightColor;
class EventAddLight;
class EventDelLight;

class LightRender : public sf::Drawable, public EventReceiver, private NonCopiable
{
public:
    LightRender();
    LightRender(LightRender &&) = default;
    LightRender & operator =(LightRender &&) = default;
    virtual ~LightRender() = default;

    void setColors(const sf::Color & base, const sf::Color & saturated = sf::Color::Black);
    void addLight(const std::weak_ptr<Light> & l);
    void delLight(const std::weak_ptr<Light> & l);

    virtual void draw(sf::RenderTarget & target, sf::RenderStates) const;

private:
    void onColorChange(EventChangeLightColor e);
    void onAddLight(EventAddLight e);
    void onDelLight(EventDelLight e);

    mutable std::unique_ptr<sf::RenderTexture> m_render;
    sf::Color m_baseColor;
    sf::Color m_saturedColor;
    mutable std::vector<std::weak_ptr<Light>> m_lights;
};

#endif // LIGHTRENDER_H

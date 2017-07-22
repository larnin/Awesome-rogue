#ifndef LIGHTHOLDER_H
#define LIGHTHOLDER_H

#include "lightdata.h"
#include "Systemes/updatable.h"
#include "Events/eventreceiver.h"
#include "Utilities/noncopiable.h"
#include "Events/Datas/Light/eventaddlight.h"
#include "Events/Datas/Light/eventdellight.h"
#include <SFML/Graphics/Color.hpp>
#include <memory>
#include <vector>

class EventSetAmbiantColor;

class LightHolder : public Updatable, public EventReceiver, private NonCopiable
{
public:
    LightHolder();
    LightHolder(LightHolder &&) = default;
    LightHolder & operator =(LightHolder &&) = default;
    virtual ~LightHolder() = default;

    void setAmbiant(const sf::Color & ambiant);
    void addLight(std::weak_ptr<LightData> light);
    void delLight(std::weak_ptr<LightData> light);

    virtual void update(const sf::Time &) override;

private:
    void updateLights();
    void onAmbiantChange(EventSetAmbiantColor e);
    void onAddLight(EventAddLight e);
    void onDelLight(EventDelLight e);

    sf::Color m_ambiantColor;
    std::vector<std::weak_ptr<LightData>> m_lights;
};

#endif // LIGHTHOLDER_H

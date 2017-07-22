#ifndef LIGHTHOLDER_H
#define LIGHTHOLDER_H

#include "Systemes/updatable.h"
#include "Events/eventreceiver.h"
#include "Types/pointlight.h"
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
    void addPointLight(std::weak_ptr<PointLight> light);
    void delPointLight(std::weak_ptr<PointLight> light);

    virtual void update(const sf::Time &) override;

private:
    void updatePointLights();
    void onAmbiantChange(EventSetAmbiantColor e);
    //void onAddPointLight(EventAddPointLight e);
    //void onDelPointLight(EventDelPointLight e);

    sf::Color m_ambiantColor;
    std::vector<std::weak_ptr<PointLight>> m_pointLights;
};

#endif // LIGHTHOLDER_H

#ifndef SMOOTHCAMERA_H
#define SMOOTHCAMERA_H

#include "Events/eventreceiver.h"
#include "Events/Datas/eventcenterofviewchanged.h"
#include "Events/Datas/eventinstantcenterofviewchanged.h"
#include "Events/Datas/eventplaycameraeffect.h"
#include "Systemes/updatable.h"

class StateMachine;

class SmoothCamera : public EventReceiver, public Updatable
{
public:
    SmoothCamera(StateMachine & m);
    virtual ~SmoothCamera() = default;

    virtual void update(const sf::Time & elapsedTime);

private:
    void onCenterChanged(EventCenterOfViewChanged e);
    void onInstantCenterChanged(EventInstantCenterOfViewChanged e);
    void onPlayCameraEffect(EventPlayCameraEffect e);

    sf::Vector2f m_currentCenter;
    sf::Vector2f m_targetCenter;
    StateMachine & m_machine;
};

#endif // SMOOTHCAMERA_H

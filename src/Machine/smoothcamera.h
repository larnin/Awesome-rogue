#ifndef SMOOTHCAMERA_H
#define SMOOTHCAMERA_H

#include <memory>
#include <vector>
#include "Events/eventreceiver.h"
#include "Events/Datas/eventcenterofviewchanged.h"
#include "Events/Datas/eventinstantcenterofviewchanged.h"
#include "Events/Datas/eventplaycameraeffect.h"
#include "Events/Datas/eventclearcameraeffects.h"
#include "Systemes/updatable.h"
#include "CameraEffects/cameraeffect.h"

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
    void onClearCameraEffects(EventClearCameraEffects e);

    std::shared_ptr<CameraEffect> createEffect(CameraEffectType type);

    sf::Vector2f m_currentCenter;
    sf::Vector2f m_targetCenter;
    StateMachine & m_machine;
    std::vector<std::pair<float, std::shared_ptr<CameraEffect>>> m_effects;
    float m_defaultZoomValue;
};

#endif // SMOOTHCAMERA_H

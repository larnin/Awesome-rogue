#ifndef SMOOTHCAMERA_H
#define SMOOTHCAMERA_H

#include "Events/eventreceiver.h"
#include "Systemes/updatable.h"
#include "CameraEffects/cameraeffecttype.h"
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <vector>

class StateMachine;
class EventCenterOfViewChanged;
class EventInstantCenterOfViewChanged;
class EventPlayCameraEffect;
class EventClearCameraEffects;
class CameraEffect;

class SmoothCamera : public EventReceiver, public Updatable
{
public:
    SmoothCamera(StateMachine & m);
    virtual ~SmoothCamera() = default;

    virtual void update(const sf::Time & elapsedTime);
    void changeDefaultZoom(float value);

private:
    void onCenterChanged(EventCenterOfViewChanged e);
    void onInstantCenterChanged(EventInstantCenterOfViewChanged e);
    void onPlayCameraEffect(EventPlayCameraEffect e);
    void onClearCameraEffects(EventClearCameraEffects);

    std::shared_ptr<CameraEffect> createEffect(CameraEffectType type);

    sf::Vector2f m_currentCenter;
    sf::Vector2f m_targetCenter;
    StateMachine & m_machine;
    std::vector<std::pair<float, std::shared_ptr<CameraEffect>>> m_effects;
    float m_defaultZoomValue;
};

#endif // SMOOTHCAMERA_H

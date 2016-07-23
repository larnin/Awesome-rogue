#include "smoothcamera.h"
#include "statemachine.h"
#include "Utilities/vect2convert.h"
#include <algorithm>
#include "Events/Datas/eventcenterofviewchanged.h"
#include "Events/Datas/eventinstantcenterofviewchanged.h"
#include "Events/Datas/eventplaycameraeffect.h"
#include "Events/Datas/eventclearcameraeffects.h"
#include "CameraEffects/effecttest.h"
#include "CameraEffects/shakeeffect.h"
#include "CameraEffects/cameraeffect.h"

SmoothCamera::SmoothCamera(StateMachine & m)
    : m_machine(m)
    , m_defaultZoomValue(0.5f)
{
    sf::RenderWindow & w(m.getWindow());
    sf::View v(w.getView());
    m_currentCenter = v.getCenter();
    m_targetCenter = v.getCenter();

    connect<EventCenterOfViewChanged>(std::bind(&onCenterChanged, this, _1));
    connect<EventInstantCenterOfViewChanged>(std::bind(&onInstantCenterChanged, this, _1));
    connect<EventPlayCameraEffect>(std::bind(&onPlayCameraEffect, this, _1));
}

void SmoothCamera::update(const sf::Time & elapsedTime)
{
    float current(norm(m_targetCenter-m_currentCenter));
    float divisor(std::pow(2.0f,4.0f*elapsedTime.asSeconds()));
    float dist(current/divisor-elapsedTime.asSeconds());
    if(dist < 0)
        dist = 0;
    m_currentCenter += toVect(current-dist, angle(m_targetCenter - m_currentCenter));

    sf::Vector2f center(m_currentCenter);
    float zoom(m_defaultZoomValue);
    float orientation(0);

    for(auto & e : m_effects)
    {
        if(!e.second)
            continue;
        if(e.first > 0)
        {
            if(elapsedTime.asSeconds() > e.first)
                e.first = 0;
            else e.first -= elapsedTime.asSeconds();
        }
        center += e.second->getOffset();
        zoom *= e.second->getZoom();
        orientation += e.second->getOrientation();
    }

    sf::RenderWindow & w(m_machine.getWindow());
    sf::View v(sf::Vector2f(sf::Vector2i(center)), sf::Vector2f(w.getSize()));
    v.setRotation(orientation);
    v.zoom(zoom);
    w.setView(v);

    auto it(std::remove_if(m_effects.begin(), m_effects.end(), [](const auto & e)
    {
        if(!e.second)
            return true;
        if(e.first < 0)
            return false;
        if(e.first == 0)
            return true;
        return false;
    }));
    m_effects.erase(it, m_effects.end());
}

void SmoothCamera::onCenterChanged(EventCenterOfViewChanged e)
{
    m_targetCenter = e.pos;
}

void SmoothCamera::onInstantCenterChanged(EventInstantCenterOfViewChanged e)
{
    m_targetCenter = e.pos;
    m_currentCenter = e.pos;
}

void SmoothCamera::onPlayCameraEffect(EventPlayCameraEffect e)
{
    m_effects.push_back(std::pair<float, std::shared_ptr<CameraEffect>>(e.time, createEffect(e.type)));
    Updatable::add(m_effects.back().second);
}

void SmoothCamera::onClearCameraEffects(EventClearCameraEffects)
{
    m_effects.clear();
}

std::shared_ptr<CameraEffect> SmoothCamera::createEffect(CameraEffectType type)
{
    switch (type)
    {
    case EFFECT_TEST:
        return std::make_shared<EffectTest>();
    case EFFECT_LOW_SHAKE:
        return std::make_shared<ShakeEffect>(2);
    case EFFECT_HARD_SHAKE:
        return std::make_shared<ShakeEffect>(7);
    default:
        assert(false);
    }
    return std::shared_ptr<CameraEffect>();
}

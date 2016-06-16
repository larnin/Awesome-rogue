#include "smoothcamera.h"
#include "statemachine.h"
#include "Utilities/vect2convert.h"

#include <iostream>

SmoothCamera::SmoothCamera(StateMachine & m)
    : m_machine(m)
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
    m_machine.setWindowCenter(m_currentCenter);
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
    //todo
}

